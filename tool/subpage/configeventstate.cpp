#include "configeventstate.h"

ConfigEventState::ConfigEventState(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void ConfigEventState::setActiveItem(FlowChartExecutorItem *item) {
    activeItem = dynamic_cast<FcExecutorItem*>(item);
    activeLine = nullptr;

    ui.input_timeout_ms->setText(QString::number(activeItem->itemData.timeoutMs));
    ui.input_retry->setText(QString::number(activeItem->itemData.timeoutRetry));
    ui.input_retry_signal_func->setText(activeItem->itemData.functionRetry);

    ui.item_config_box->setVisible(true);
    ui.line_config_box->setVisible(false);
}

void ConfigEventState::setActiveLine(FcConnectLine *line) {
    activeLine = line;
    activeItem = nullptr;

    ui.input_trigger_func->setText(activeLine->lineData.functionTrigger);
    ui.input_signal_check->setText(activeLine->lineData.functionCheck);
    ui.check_fail_branch->setChecked(activeLine->lineData.failBranch);

    ui.item_config_box->setVisible(false);
    ui.line_config_box->setVisible(true);
}

void ConfigEventState::on_input_timeout_ms_editingFinished() {
    if (!activeItem) return;
    activeItem->itemData.timeoutMs = ui.input_timeout_ms->text().toInt();
}

void ConfigEventState::on_input_retry_editingFinished() {
    if (!activeItem) return;
    activeItem->itemData.timeoutRetry = ui.input_retry->text().toInt();
}

void ConfigEventState::on_input_retry_signal_func_editingFinished() {
    if (!activeItem) return;
    activeItem->itemData.functionRetry = ui.input_retry_signal_func->text();
}

void ConfigEventState::on_input_trigger_func_editingFinished() {
    if (!activeLine) return;
    activeLine->lineData.functionTrigger = ui.input_trigger_func->text();
}

void ConfigEventState::on_input_signal_check_editingFinished() {
    if (!activeLine) return;
    activeLine->lineData.functionCheck = ui.input_signal_check->text();
}

void ConfigEventState::on_check_fail_branch_stateChanged(int state) {
    if (!activeLine) return;
    activeLine->lineData.failBranch = state == Qt::Checked;
    viewChanged();
}

