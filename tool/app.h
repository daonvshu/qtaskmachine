#pragma once

#include <qwidget.h>
#include <qstandarditemmodel.h>

#include "ui_app.h"

#include "data/configflows.h"

#include "flowchart/elements/flowchartexecutoritem.h"

#include "subpage/commonpropmanager.h"

class App : public QWidget {
    Q_OBJECT

public:
    explicit App(QWidget *parent = nullptr);

private:
    Ui::App ui;

    ConfigFlowGroup flowGroup;
    QStandardItemModel* flowsMode;

    CommonPropManager* commonPropManager;

    QString configFilePath;

private slots:
    void on_btn_new_config_clicked();
    void on_btn_open_config_clicked();
    void on_flow_list_clicked(const QModelIndex &index);
    void on_btn_create_new_clicked();
    void on_btn_remove_item_clicked();
    void on_btn_save_item_clicked();

private:
    void itemNameChanged(const QModelIndex& index, const QString& text);
    void flowItemRemove(int index);
    void saveFlowConfig(const QList<QGraphicsItem*>& items);
    void saveConfigToFile();
    void refreshConfigPathLabel();
    void createNewConfig(const QString& filePath);
    void openExistConfig(const QString& filePath);
    void reloadFlowList();
    void reloadFlowConfig(int rowIndex);

    void nodeSelected(FlowChartExecutorItem* item);
};