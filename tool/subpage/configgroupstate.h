#pragma once

#include <qwidget.h>

#include "stateconfiginterface.h"

#include "ui_configgroupstate.h"

class ConfigGroupState : public QWidget, public StateConfigInterface {
    Q_OBJECT

public:
    explicit ConfigGroupState(QWidget *parent = nullptr);

    void setActiveItem(FlowChartExecutorItem *item) override;

    void setActiveLine(FcConnectLine *line) override;

private slots:
    void on_branch_type_group_buttonClicked(QAbstractButton *button);
    void on_input_trigger_func_editingFinished();

private:
    void reloadEnableState();

private:
    Ui::ConfigGroupState ui;
    FcConnectLine* activeLine = nullptr;
};
