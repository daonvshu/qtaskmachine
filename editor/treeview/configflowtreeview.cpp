#include "configflowtreeview.h"
#include "treeviewitemnode.h"
#include "styledmenu.h"

#include "dialogs/messagedlg.h"

#include <qevent.h>

ConfigFlowTreeView::ConfigFlowTreeView(QWidget* parent)
    : QTreeView(parent)
{
    rootNode = TreeViewItemNode::createRootItem(this, this, "rootNode");

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &ConfigFlowTreeView::onContextMenuRequest);
    connect(selectionModel(), &QItemSelectionModel::currentChanged, this, &ConfigFlowTreeView::onChildItemClicked);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(InternalMove);
    setDropIndicatorShown(true);
    setEditTriggers(NoEditTriggers);

    qRegisterMetaType<ConfigFlow*>();
}

void ConfigFlowTreeView::setData(ConfigFlowGroup* flowGroup) {
    flowGroupDataPtr = flowGroup;
}

void ConfigFlowTreeView::reloadTree() {
    rootNode->clearChild();
    lastClickedFlowIndex = -1;
    removedFlowIndex.clear();
    removedGroupIndex.clear();

    QHash<QString, TreeViewItemNode*> groupNameToNode;
    for (int i = 0; i < flowGroupDataPtr->groups().size(); ++i) {
        auto name = flowGroupDataPtr->groups()[i];
        auto node = rootNode->add(name.isEmpty() ? tr("[未分组]") : name, i);
        groupNameToNode.insert(name, node);
    }

    for (int i = 0; i < flowGroupDataPtr->flows().size(); ++i) {
        const auto& flow = flowGroupDataPtr->flows()[i];
        if (!groupNameToNode.contains(flow.group())) {
            flowGroupDataPtr->groups() << flow.group();
            auto node = rootNode->add(flow.group().isEmpty() ? tr("[未分组]") : flow.group(), flowGroupDataPtr->groups().size() - 1);
            groupNameToNode.insert(flow.group(), node);
        }
        groupNameToNode.value(flow.group())->add(flow.name(), i);
    }

    expandAll();
}

ConfigFlow* ConfigFlowTreeView::getCurrentSelectedFlow() const {
    if (lastClickedFlowIndex == -1) {
        return nullptr;
    }
    return &flowGroupDataPtr->flows()[lastClickedFlowIndex];
}

void ConfigFlowTreeView::saveConfig() {
    if (flowGroupDataPtr->configFilePath.isEmpty()) {
        return;
    }

    QFile file(flowGroupDataPtr->configFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        MessageDlg::showMessage(QStringLiteral("错误"), QStringLiteral("文件无法写入！"), this);
        return;
    }

    ConfigFlowGroup tmpFlow;
    //按照节点树顺序添加数据
    for (const auto& groupNode : rootNode->childItems()) {
        int groupIndex = groupNode->itemData<int>();
        tmpFlow.groups() << flowGroupDataPtr->groups()[groupIndex];
        for (const auto& flowNode : groupNode->childItems()) {
            int flowIndex = flowNode->itemData<int>();
            auto& flow = flowGroupDataPtr->flows()[flowIndex];
            flow.group = tmpFlow.groups().last(); //flow可能被移动到其他组
            tmpFlow.flows() << flow;
        }
    }

    auto obj = tmpFlow.dumpToJson();
    auto data = QJsonDocument(obj).toJson(QJsonDocument::Indented);
    file.write(data);
    file.close();
}

void ConfigFlowTreeView::onContextMenuRequest(const QPoint& pos) {

    auto index = indexAt(pos);
    if (index.isValid()) {
        auto node = TreeViewItemNode::fromIndex(index);
        if (node->parentNode() == rootNode) {
            showGroupMenu(node);
        } else {
            showFlowMenu(node);
        }
        return;
    }

    auto styledMenu = new StyledMenu({
        { ":/res/add.svg", tr("添加组") },
    }, this);
    connect(styledMenu, &StyledMenu::onMenuItemClicked, this, [&] (int i) {
        switch (i) {
            case 0: createNewGroup(); break;
            default:
                break;
        }
    });
    styledMenu->moveShow(QCursor::pos());
}

void ConfigFlowTreeView::showGroupMenu(TreeViewItemNode* node) {
    auto styledMenu = new StyledMenu({
                                         {":/res/edit.svg", tr("修改名称")},
                                         {":/res/delete.svg", tr("删除组")},
                                         {":/res/add.svg", tr("添加项")}
                                     }, this);
    connect(styledMenu, &StyledMenu::onMenuItemClicked, this, [=] (int index) {
        switch (index) {
            case 0: modifyGroupName(node); break;
            case 1: deleteGroup(node); break;
            case 2: addFlowItem(node); break;
            default:
                break;
        }
    });
    styledMenu->moveShow(QCursor::pos());
}

void ConfigFlowTreeView::showFlowMenu(TreeViewItemNode* node) {
    auto styledMenu = new StyledMenu({
                                         {":/res/edit.svg", tr("修改")},
                                         {":/res/delete.svg", tr("删除")},
                                         {":/res/copy.svg", tr("复制")}
                                     }, this);
    connect(styledMenu, &StyledMenu::onMenuItemClicked, this, [=] (int index) {
        switch (index) {
            case 0: modifyFlowName(node); break;
            case 1: deleteFlow(node); break;
            case 2: copyFlow(node); break;
            default:
                break;
        }
    });
    styledMenu->moveShow(QCursor::pos());
}

void ConfigFlowTreeView::onChildItemClicked(const QModelIndex& index) {
    if (index.isValid()) {
        auto node = TreeViewItemNode::fromIndex(index);
        if (node->parentNode() == rootNode) {
            return;
        }
        lastClickedFlowIndex = node->itemData<int>();
        emit flowClicked(&flowGroupDataPtr->flows()[lastClickedFlowIndex]);
    }
}

void ConfigFlowTreeView::startDrag(Qt::DropActions supportedActions) {
    QTreeView::startDrag(supportedActions);
    saveConfig();
}
