#include "configgroupstate.h"

ConfigGroupState::ConfigGroupState(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void ConfigGroupState::setActiveItem(FlowChartExecutorItem *item) {

}

void ConfigGroupState::setActiveLine(FcConnectLine *line) {
    activeLine = line;

    if (activeLine->lineData.failBranch) {
        ui.check_fail_branch->setChecked(true);
    } else if (activeLine->lineData.subBranch) {
        ui.check_sub_state_branch->setChecked(true);
    } else {
        ui.check_next_branch->setChecked(true);
    }

    ui.input_trigger_func->setText(activeLine->lineData.functionTrigger);

    reloadEnableState();
}

void ConfigGroupState::on_branch_type_group_buttonClicked(QAbstractButton *button) {
    if (!activeLine) return;
    if (button == ui.check_next_branch) {
        activeLine->lineData.failBranch = false;
        activeLine->lineData.subBranch = false;
    } else if (button == ui.check_fail_branch) {
        activeLine->lineData.failBranch = true;
        activeLine->lineData.subBranch = false;
    } else if (button == ui.check_sub_state_branch) {
        activeLine->lineData.failBranch = false;
        activeLine->lineData.subBranch = true;
    }
    reloadEnableState();
}

void ConfigGroupState::on_input_trigger_func_editingFinished() {
    if (!activeLine) return;
    activeLine->lineData.functionTrigger = ui.input_trigger_func->text();
}

void ConfigGroupState::reloadEnableState() {
    bool enabled = activeLine->lineData.failBranch;
    ui.label_trigger->setEnabled(enabled);
    ui.input_trigger_func->setEnabled(enabled);
    ui.label_trigger_hint->setEnabled(enabled);
    viewChanged();
}
