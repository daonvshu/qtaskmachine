#pragma once

#include <qtreeview.h>
#include <qvariant.h>
#include <qobject.h>

#include "treeviewitemmodel.h"

class TreeViewItemNode : public QObject {
public:
    static TreeViewItemNode* createRootItem(QTreeView* treeView, QObject* callbackContext, const QString& displayText = QString());

    ~TreeViewItemNode();

    QString displayText() const;

    void resetDisplayText(const QString& text);

    void resetData(const QVariant& data);

    TreeViewItemNode* add(const QString& displayText, const QVariant& data = QVariant());

    void remove();

    void clearChild();

    QList<TreeViewItemNode*> childItems();

    TreeViewItemNode* childAt(int index);

    TreeViewItemNode* parentNode() const;

    QModelIndex modelIndex();

    TreeViewItemModel* model() const;

    static TreeViewItemNode* fromIndex(const QModelIndex& index);

    template<typename K>
    using MenuItemPtr = std::tuple<QString, QString, void(K::*)(TreeViewItemNode*)>;

    template<typename T>
    T itemData() const {
        return m_itemData.value<T>();
    }

private:
    TreeViewItemNode(const QString& displayText, TreeViewItemModel* treeModel, TreeViewItemNode* parentItem = nullptr);

    void appendChild(TreeViewItemNode *child);

    void removeChild(TreeViewItemNode* child);

    void eraseChild(int index);

    TreeViewItemNode *child(int row) const;

    int childCount() const;

    int row() const;

private:
    QList<TreeViewItemNode*> m_childItems;
    QVariant m_itemData;
    TreeViewItemNode *m_parentItem;
    QString m_displayText;

    TreeViewItemModel* treeModel;

    friend class TreeViewItemModel;
};