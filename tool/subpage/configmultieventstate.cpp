#include "configmultieventstate.h"

ConfigMultiEventState::ConfigMultiEventState(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void ConfigMultiEventState::setActiveItem(FlowChartExecutorItem *item) {
    ui.line_config_box->setVisible(false);
}

void ConfigMultiEventState::setActiveLine(FcConnectLine *line) {
    activeLine = line;

    ui.input_trigger_func->setText(activeLine->lineData.functionTrigger);
    ui.input_signal_check->setText(activeLine->lineData.functionCheck);
    ui.input_branch_id->setText(QString::number(activeLine->lineData.branchId));

    ui.line_config_box->setVisible(true);
}

void ConfigMultiEventState::on_input_trigger_func_editingFinished() {
    if (!activeLine) return;
    activeLine->lineData.functionTrigger = ui.input_trigger_func->text();
}

void ConfigMultiEventState::on_input_signal_check_editingFinished() {
    if (!activeLine) return;
    activeLine->lineData.functionCheck = ui.input_signal_check->text();
}

void ConfigMultiEventState::on_input_branch_id_editingFinished() {
    if (!activeLine) return;
    activeLine->lineData.branchId = ui.input_branch_id->text().toInt();
}
