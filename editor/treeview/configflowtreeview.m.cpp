#include "configflowtreeview.h"
#include "treeviewitemnode.h"

#include "dialogs/nameeditdlg.h"
#include "dialogs/messagedlg.h"

void ConfigFlowTreeView::createNewGroup() {
    auto newName = NameEditDlg::showDialog("", tr("添加组"), tr("组名称"), this);
    if (newName.isEmpty()) {
        return;
    }
    if (flowGroupDataPtr->groups().contains(newName)) {
        MessageDlg::showMessage(tr("错误"), tr("组名已存在"), this);
        return;
    }
    flowGroupDataPtr->groups() << newName;
    rootNode->add(newName, flowGroupDataPtr->groups().size() - 1);
    saveConfig();
}

void ConfigFlowTreeView::modifyGroupName(TreeViewItemNode* node) {
    int groupIndex = node->itemData<int>();
    auto& groupName = flowGroupDataPtr->groups()[groupIndex];
    auto newName = NameEditDlg::showDialog(groupName, tr("修改组名"), tr("组名称"), this);
    if (!newName.isEmpty()) {
        if (flowGroupDataPtr->groups().contains(newName)) {
            MessageDlg::showMessage(tr("错误"), tr("组名已存在"), this);
            return;
        }
        for (auto& flow : flowGroupDataPtr->flows()) {
            if (flow.group() == groupName) {
                flow.group = newName;
            }
        }
        groupName = newName;
        node->resetDisplayText(newName);
        saveConfig();
    }
}

void ConfigFlowTreeView::deleteGroup(TreeViewItemNode* node) {
    int groupIndex = node->itemData<int>();
    auto groupName = flowGroupDataPtr->groups()[groupIndex];
    auto dlg = MessageDlg::showMessage(tr("警告"), tr("确定删除组‘%1’和该组下所有流程？").arg(groupName), this, true);
    connect(dlg, &MessageDlg::accepted, this, [&, node] {
        //不能立即删除，否则会造成其他项index映射错误
        node->remove();
        saveConfig();
    });
}

void ConfigFlowTreeView::addFlowItem(TreeViewItemNode* node) {
    auto newFlowName = NameEditDlg::showDialog(QString(), tr("新建流程"), tr("流程名称"), this);
    if (!newFlowName.isEmpty()) {
        if (isFlowExist(newFlowName)) {
            MessageDlg::showMessage(tr("错误"), tr("流程已存在！"), this);
            return;
        }
        ConfigFlow configFlow;
        configFlow.name = newFlowName;
        configFlow.group = node->itemData<QString>();
        flowGroupDataPtr->flows().append(configFlow);

        auto newNode = node->add(newFlowName, flowGroupDataPtr->flows().size() - 1);
        auto index = newNode->modelIndex();
        setCurrentIndex(index);
        onChildItemClicked(index);
        saveConfig();
    }
}

void ConfigFlowTreeView::modifyFlowName(TreeViewItemNode* node) {
    int flowIndex = node->itemData<int>();
    auto& flow = flowGroupDataPtr->flows()[flowIndex];
    auto newFlowName = NameEditDlg::showDialog(flow.name(), tr("流程名称"), tr("重命名流程"), this);
    if (!newFlowName.isEmpty()) {
        if (isFlowExist(newFlowName)) {
            MessageDlg::showMessage(tr("错误"), tr("流程已存在！"), this);
            return;
        }
        flow.name = newFlowName;
        node->resetDisplayText(newFlowName);
        saveConfig();
    }
}

void ConfigFlowTreeView::deleteFlow(TreeViewItemNode* node) {
    int flowIndex = node->itemData<int>();
    const auto& flow = flowGroupDataPtr->flows()[flowIndex];
    auto dlg = MessageDlg::showMessage(tr("警告"), tr("确定删除流程‘%1’？").arg(flow.name()), this, true);
    connect(dlg, &MessageDlg::accepted, this, [&, node] {
        //不能立即删除，否则会造成其他项index映射错误
        node->remove();
        saveConfig();
    });
}

void ConfigFlowTreeView::copyFlow(TreeViewItemNode* node) {
    int flowIndex = node->itemData<int>();
    const auto& flow = flowGroupDataPtr->flows()[flowIndex];
    auto newFlowName = NameEditDlg::showDialog(flow.name(), tr("复制流程"), tr("流程名称"), this);
    if (!newFlowName.isEmpty()) {
        if (isFlowExist(newFlowName)) {
            MessageDlg::showMessage(tr("错误"), tr("流程已存在！"), this);
            return;
        }
        ConfigFlow configFlow = flow;
        configFlow.name = newFlowName;
        flowGroupDataPtr->flows().append(configFlow);
        for (auto& groupNode : rootNode->childItems()) {
            auto groupName = flowGroupDataPtr->groups()[groupNode->itemData<int>()];
            if (groupName == flow.group()) {
                groupNode->add(newFlowName, flowGroupDataPtr->flows().size() - 1);
                break;
            }
        }
        saveConfig();
    }
}

bool ConfigFlowTreeView::isFlowExist(const QString& name) const {
    for (auto& flow : flowGroupDataPtr->flows()) {
        if (flow.name() == name) {
            return true;
        }
    }
    return false;
}
