#pragma once

#include <qwidget.h>

#include "stateconfiginterface.h"

#include "ui_configmultieventstate.h"

class ConfigMultiEventState : public QWidget, public StateConfigInterface {
    Q_OBJECT

public:
    explicit ConfigMultiEventState(QWidget *parent = nullptr);

    void setActiveItem(FlowChartExecutorItem *item) override;

    void setActiveLine(FcConnectLine *line) override;

private slots:
    void on_input_trigger_func_editingFinished();
    void on_input_signal_check_editingFinished();

private:
    Ui::ConfigMultiEventState ui;
    FcConnectLine* activeLine = nullptr;
};
