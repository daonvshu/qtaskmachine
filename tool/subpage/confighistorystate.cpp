#include "confighistorystate.h"

ConfigHistoryState::ConfigHistoryState(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void ConfigHistoryState::setActiveItem(FlowChartExecutorItem *item) {
    activeItem = dynamic_cast<FcExecutorItem*>(item);
    ui.check_nested->setChecked(activeItem->itemData.nested);
}

void ConfigHistoryState::setActiveLine(FcConnectLine *line) {
}

void ConfigHistoryState::on_check_nested_stateChanged(int state) {
    if (!activeItem) return;
    activeItem->itemData.nested = state == Qt::Checked;
}
