#pragma once

#include <qwidget.h>

#include "stateconfiginterface.h"

#include "ui_configdelaystate.h"

class ConfigDelayState : public QWidget, public StateConfigInterface {
    Q_OBJECT

public:
    explicit ConfigDelayState(QWidget *parent = nullptr);

    void setActiveItem(FlowChartExecutorItem *item) override;

private slots:
    void on_input_delay_ms_editingFinished();

private:
    Ui::ConfigDelayState ui;
    FcExecutorItem* activeItem = nullptr;
};
