#pragma once

#include <qwidget.h>

#include "stateconfiginterface.h"

#include "ui_configeventstate.h"

class ConfigEventState : public QWidget, public StateConfigInterface {
    Q_OBJECT

public:
    explicit ConfigEventState(QWidget *parent = nullptr);

    void setActiveItem(FlowChartExecutorItem *item) override;

    void setActiveLine(FcConnectLine *line) override;

private slots:
    void on_input_timeout_ms_editingFinished();
    void on_input_retry_editingFinished();
    void on_input_retry_signal_func_editingFinished();
    void on_input_trigger_func_editingFinished();
    void on_check_fail_branch_stateChanged(int state);

private:
    Ui::ConfigEventState ui;
    FcExecutorItem* activeItem = nullptr;
    FcConnectLine* activeLine = nullptr;
};
