#include "configdelaystate.h"

ConfigDelayState::ConfigDelayState(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void ConfigDelayState::setActiveItem(FlowChartExecutorItem *item) {
    activeItem = dynamic_cast<FcExecutorItem*>(item);

    ui.input_delay_ms->setText(QString::number(activeItem->itemData.delayMs));
}

void ConfigDelayState::on_input_delay_ms_editingFinished() {
    activeItem->itemData.delayMs = ui.input_delay_ms->text().toInt();
}

void ConfigDelayState::setActiveLine(FcConnectLine *line) {
}
