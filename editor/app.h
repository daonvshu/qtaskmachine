#pragma once

#include <qwidget.h>

#include "ui_app.h"

#include "data/configflows.h"

class App : public QWidget {
    Q_OBJECT

public:
    explicit App(QWidget *parent = nullptr);

private:
    Ui::App ui;
    ConfigFlowGroup flowGroup;
    QString configFilePath;

private slots:
    void on_btn_min_clicked();
    void on_btn_max_clicked();
    void on_btn_close_clicked();

    void on_btn_new_config_clicked();
    void on_btn_open_config_clicked();
    void on_flow_list_cb_currentIndexChanged(int index);
    void on_btn_flow_add_clicked();
    void on_btn_flow_edit_clicked();
    void on_btn_flow_remove_clicked();
    void on_graphic_view_configChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void refreshConfigPathLabel();
    void reloadFlowList();
    void createNewConfig(const QString& filePath);
    void openExistConfig(const QString& filePath);
    void saveConfigToFile();
    void updateFlowListWidth();
    static void saveLastOpenFilePathRecord(const QString& filePath);
};
