#include "treeviewitemnode.h"

TreeViewItemNode* TreeViewItemNode::createRootItem(QTreeView* treeView, QObject* callbackContext, const QString& displayText) {
    auto root = new TreeViewItemNode(displayText, nullptr, nullptr);
    root->setParent(callbackContext);
    root->m_displayText = displayText;
    root->treeModel = new TreeViewItemModel(root, callbackContext);
    treeView->setModel(root->treeModel);
    return root;
}

TreeViewItemNode::TreeViewItemNode(const QString &displayText, TreeViewItemModel *treeModel, TreeViewItemNode *parentItem)
    : m_parentItem(parentItem)
    , m_displayText(displayText)
    , treeModel(treeModel)
{}

TreeViewItemNode::~TreeViewItemNode()  {
    qDeleteAll(m_childItems);
}

QString TreeViewItemNode::displayText() const {
    return m_displayText;
}

void TreeViewItemNode::resetDisplayText(const QString &text) {
    m_displayText = text;
    treeModel->displayChanged(this);
}

void TreeViewItemNode::resetData(const QVariant& data) {
    m_itemData = data;
    treeModel->itemDataChanged(this);
}

TreeViewItemNode *TreeViewItemNode::add(const QString &displayText, const QVariant& data) {
    auto item = new TreeViewItemNode(displayText, treeModel, this);
    item->m_itemData = data;
    treeModel->addChild(item);
    return item;
}

void TreeViewItemNode::remove() {
    treeModel->removeItem(this);
}

void TreeViewItemNode::clearChild() {
    for (int i = m_childItems.size() - 1; i >= 0; i--) {
        m_childItems.at(i)->remove();
    }
}

QList<TreeViewItemNode*> TreeViewItemNode::childItems() {
    return m_childItems;
}

TreeViewItemNode *TreeViewItemNode::childAt(int index) {
    if (index < 0 || index >= m_childItems.size()) {
        return nullptr;
    }
    return m_childItems[index];
}

TreeViewItemNode* TreeViewItemNode::parentNode() const {
    return m_parentItem;
}

QModelIndex TreeViewItemNode::modelIndex() {
    return treeModel->index(this);
}

TreeViewItemModel* TreeViewItemNode::model() const {
    return treeModel;
}

TreeViewItemNode* TreeViewItemNode::fromIndex(const QModelIndex& index) {
    return static_cast<TreeViewItemNode*>(index.internalPointer());
}

void TreeViewItemNode::appendChild(TreeViewItemNode *child)  {
    m_childItems.append(child);
}

void TreeViewItemNode::removeChild(TreeViewItemNode *child) {
    m_childItems.removeOne(child);
}

void TreeViewItemNode::eraseChild(int index) {
    delete m_childItems.takeAt(index);
}

TreeViewItemNode *TreeViewItemNode::child(int row) const {
    if (row < 0 || row >= m_childItems.size()) {
        return nullptr;
    }
    return m_childItems.at(row);
}

int TreeViewItemNode::childCount() const {
    return m_childItems.count();
}

int TreeViewItemNode::row() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeViewItemNode*>(this));
    return 0;
}
