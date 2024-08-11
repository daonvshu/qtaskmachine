#pragma once

#include <qwidget.h>

#include "stateconfiginterface.h"

#include "ui_confighistorystate.h"

class ConfigHistoryState : public QWidget, public StateConfigInterface {
    Q_OBJECT

public:
    explicit ConfigHistoryState(QWidget *parent = nullptr);

    void setActiveItem(FlowChartExecutorItem *item) override;

    void setActiveLine(FcConnectLine *line) override;

private slots:
    void on_check_nested_stateChanged(int state);

private:
    Ui::ConfigHistoryState ui;
    FcExecutorItem* activeItem = nullptr;
};
