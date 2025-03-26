#pragma once

#include <qtreeview.h>

#include "data/configflows.h"

class TreeViewItemNode;
class ConfigFlowTreeView : public QTreeView {
    Q_OBJECT

public:
    explicit ConfigFlowTreeView(QWidget* parent = nullptr);

    void setData(ConfigFlowGroup* flowGroup);

    void reloadTree();

    ConfigFlow* getCurrentSelectedFlow() const;

    void saveConfig();

    void createNewGroup();

signals:
    void flowClicked(ConfigFlow* flow);

private:
    ConfigFlowGroup* flowGroupDataPtr = nullptr;
    TreeViewItemNode* rootNode;
    int lastClickedFlowIndex = -1;

private:
    void onContextMenuRequest(const QPoint& pos);

    void showGroupMenu(TreeViewItemNode* node);
    void showFlowMenu(TreeViewItemNode* node);

    void onChildItemClicked(const QModelIndex& current);

    void modifyGroupName(TreeViewItemNode* node);
    void deleteGroup(TreeViewItemNode* node);
    void addFlowItem(TreeViewItemNode* node);

    void modifyFlowName(TreeViewItemNode* node);
    void deleteFlow(TreeViewItemNode* node);
    void copyFlow(TreeViewItemNode* node);

private:
    bool isFlowExist(const QString& name) const;

protected:
    void startDrag(Qt::DropActions supportedActions) override;
};

Q_DECLARE_METATYPE(ConfigFlow*);
