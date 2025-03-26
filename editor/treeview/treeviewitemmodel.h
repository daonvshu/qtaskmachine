#pragma once

#include <qabstractitemmodel.h>

class TreeViewItemNode;
class TreeViewItemModel : public QAbstractItemModel {
public:
    explicit TreeViewItemModel(TreeViewItemNode* rootItem, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    bool removeRows(int row, int count, const QModelIndex &parent) override;

    bool hasChildren(const QModelIndex& parent) const override;

    void addChild(TreeViewItemNode* item);

    void displayChanged(TreeViewItemNode* item);

    void itemDataChanged(TreeViewItemNode* item);

    void removeItem(TreeViewItemNode* item);

    QModelIndex index(TreeViewItemNode* item);

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    Qt::DropActions supportedDropActions() const override;

    QStringList mimeTypes() const override;

    QMimeData* mimeData(const QModelIndexList& indexes) const override;

    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

    static int calculateDepth(const QModelIndex& index);

private:
    TreeViewItemNode* rootItem;
};
