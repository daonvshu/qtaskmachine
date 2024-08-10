#include "configconditionstate.h"

ConfigConditionState::ConfigConditionState(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void ConfigConditionState::setActiveItem(FlowChartExecutorItem *item) {
    activeItem = dynamic_cast<FcConditionalItem*>(item);
    activeLine = nullptr;

    ui.input_condition_func->setText(activeItem->conditionData.functionCondition);

    ui.item_config_box->setVisible(true);
    ui.line_config_box->setVisible(false);
}

void ConfigConditionState::setActiveLine(FcConnectLine *line) {
    activeLine = line;
    activeItem = nullptr;

    ui.input_branch_id->setText(QString::number(activeLine->lineData.branchId));

    ui.item_config_box->setVisible(false);
    ui.line_config_box->setVisible(true);
}

void ConfigConditionState::on_input_condition_func_editingFinished() {
    if (activeItem == nullptr) return;
    activeItem->conditionData.functionCondition = ui.input_condition_func->text();
}

void ConfigConditionState::on_input_branch_id_editingFinished() {
    if (activeLine == nullptr) return;
    activeLine->lineData.branchId = ui.input_branch_id->text().toInt();
}
