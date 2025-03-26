#include "treeviewitemmodel.h"
#include "treeviewitemnode.h"

#include <qdatastream.h>
#include <qmimedata.h>
#include <qdebug.h>

TreeViewItemModel::TreeViewItemModel(TreeViewItemNode* rootItem, QObject *parent)
    : QAbstractItemModel(parent)
    , rootItem(rootItem)
{
}

QVariant TreeViewItemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return {};

    auto item = static_cast<TreeViewItemNode*>(index.internalPointer());
    if (role == Qt::DisplayRole)
        return item->m_displayText;
    if (role == Qt::UserRole)
        return QVariant::fromValue(item);
    return {};
}

QModelIndex TreeViewItemModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return {};

    TreeViewItemNode *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeViewItemNode*>(parent.internalPointer());

    auto childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return {};
}

QModelIndex TreeViewItemModel::parent(const QModelIndex &child) const {
    if (!child.isValid())
        return {};

    auto childItem = static_cast<TreeViewItemNode*>(child.internalPointer());
    if (childItem == rootItem)
        return {};
    auto parentItem = childItem->m_parentItem;

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeViewItemModel::rowCount(const QModelIndex &parent) const {
    TreeViewItemNode *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeViewItemNode*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeViewItemModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

bool TreeViewItemModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);
    TreeViewItemNode* parentItem;
    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<TreeViewItemNode *>(parent.internalPointer());
    }
    for (int i = row + count - 1; i >= row; i--) {
        parentItem->eraseChild(i);
    }
    endRemoveRows();
    return true;
}

bool TreeViewItemModel::hasChildren(const QModelIndex& parent) const {
    return rowCount(parent) > 0;
}

void TreeViewItemModel::addChild(TreeViewItemNode *item) {
    auto parentItem = item->m_parentItem;
    auto parentIndex = index(parentItem);
    beginInsertRows(parentIndex, parentItem->childCount(), parentItem->childCount());
    parentItem->appendChild(item);
    endInsertRows();
    emit layoutChanged();
}

void TreeViewItemModel::displayChanged(TreeViewItemNode *item) {
    auto nodeIndex = index(item);
    emit dataChanged(nodeIndex, nodeIndex, { Qt::DisplayRole });
}

void TreeViewItemModel::itemDataChanged(TreeViewItemNode *item) {
    auto nodeIndex = index(item);
    emit dataChanged(nodeIndex, nodeIndex, { Qt::UserRole + 1 });
}

void TreeViewItemModel::removeItem(TreeViewItemNode *item) {
    if (item == nullptr || item == rootItem) {
        return;
    }
    auto parentItem = item->m_parentItem;
    auto parentIndex = index(parentItem);
    int row = item->row();
    beginRemoveRows(parentIndex, row, row);
    parentItem->removeChild(item);
    endRemoveRows();
}

QModelIndex TreeViewItemModel::index(TreeViewItemNode *item) {
    return createIndex(item->row(), 0, item);
}

Qt::ItemFlags TreeViewItemModel::flags(const QModelIndex& index) const {
    auto flags = QAbstractItemModel::flags(index);
    return flags | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

Qt::DropActions TreeViewItemModel::supportedDropActions() const {
    return Qt::MoveAction;
}

QStringList TreeViewItemModel::mimeTypes() const {
    return { "application/x-customtreeviewitemdatalist" };
}

QMimeData* TreeViewItemModel::mimeData(const QModelIndexList& indexes) const {
    if (indexes.count() <= 0)
        return nullptr;
    auto mimeData = new QMimeData();
    QByteArray encoded;
    QDataStream stream(&encoded, QIODevice::WriteOnly);
    for (auto& index : indexes) {
        auto node = static_cast<TreeViewItemNode*>(index.internalPointer());
        stream << index.row()
                << node->m_itemData
                << node->m_displayText
                << calculateDepth(index);
        stream << node->childCount();
        for (auto child : node->childItems()) {
            stream << child->m_itemData
                    << child->m_displayText;
        }
    }
    mimeData->setData(mimeTypes()[0], encoded);
    return mimeData;
}

bool TreeViewItemModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) {
    if (!data || !supportedDropActions().testFlag(action)) {
        return false;
    }
    auto format = mimeTypes()[0];
    if (!data->hasFormat(format)) {
        return false;
    }
    if (!parent.isValid()) {
        return false;
    }
    if (row > rowCount(parent))
        row = rowCount(parent);
    if (row == -1)
        row = rowCount(parent);
    if (column == -1)
        column = 0;
    auto tagIndexDepth = calculateDepth(parent) + 1;
    auto encoded = data->data(format);
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    QList<int> dataRows;
    QVariantList itemDatas;
    QStringList displayTexts;
    QList<QList<QVariant>> childItemDatas;
    QList<QStringList> childDisplayTexts;
    while (!stream.atEnd()) {
        int dataRow;
        QVariant itemData;
        QString displayText;
        int depth;
        int childCount;
        stream >> dataRow >> itemData >> displayText >> depth >> childCount;
        QVariantList curChildItemData;
        QStringList curChildDisplayTexts;
        for (int i = 0; i < childCount; i++) {
            QVariant childItemData;
            QString childDisplayText;
            stream >> childItemData >> childDisplayText;
            curChildItemData << childItemData;
            curChildDisplayTexts << childDisplayText;
        }
        if (depth != tagIndexDepth) {
            return false;
        }
        dataRows.append(dataRow);
        itemDatas.append(itemData);
        displayTexts.append(displayText);
        childItemDatas.append(curChildItemData);
        childDisplayTexts.append(curChildDisplayTexts);
    }

    beginInsertRows(parent, row, row + dataRows.count() - 1);
    auto parentItem = static_cast<TreeViewItemNode*>(parent.internalPointer());
    for (int i = 0; i < dataRows.count(); i++) {
        auto item = new TreeViewItemNode(displayTexts[i], rootItem->treeModel, parentItem);
        item->m_itemData = itemDatas[i];
        parentItem->m_childItems.insert(row + i, item);
        for (int j = 0; j < childItemDatas[i].count(); j++) {
            auto childItem = new TreeViewItemNode(childDisplayTexts[i][j], rootItem->treeModel, item);
            childItem->m_itemData = childItemDatas[i][j];
            item->m_childItems.append(childItem);
        }
    }
    endInsertRows();
    return true;
}

int TreeViewItemModel::calculateDepth(const QModelIndex& index) {
    int depth = 0;
    QModelIndex currentIndex = index;
    while (currentIndex.isValid()) {
        depth++;
        currentIndex = currentIndex.parent();
    }
    return depth;
}
