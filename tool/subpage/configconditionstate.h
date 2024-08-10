#pragma once

#include <qwidget.h>

#include "stateconfiginterface.h"

#include "ui_configconditionstate.h"

class ConfigConditionState : public QWidget, public StateConfigInterface {
    Q_OBJECT

public:
    explicit ConfigConditionState(QWidget *parent = nullptr);

    void setActiveItem(FlowChartExecutorItem *item) override;

    void setActiveLine(FcConnectLine *line) override;

private slots:
    void on_input_condition_func_editingFinished();
    void on_input_branch_id_editingFinished();

private:
    Ui::ConfigConditionState ui;
    FcConditionalItem* activeItem = nullptr;
    FcConnectLine* activeLine = nullptr;
};
