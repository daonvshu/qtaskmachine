#pragma once

#include <qdialog.h>

#include "ui_monitordlg.h"

#include "remote/remotecontrol.h"
#include "data/configflows.h"

class MonitorDlg : public QDialog {
    Q_OBJECT

public:
    explicit MonitorDlg(RemoteControl* remoteControl, ConfigFlow* currentFlow, QWidget *parent = nullptr);

    void flowChanged(ConfigFlow* flow);

signals:
    void requestSelectNode(const QString& flowName, const QString& uuid);

private:
    Ui::MonitorDlg ui;
    RemoteControl* remoteControl;
    ConfigFlow* currentFlow;

private slots:
    void on_btn_close_clicked();
    void on_btn_confirm_clicked();
    void on_btn_connect_clicked();
    void on_btn_backtrack_clicked();
    void on_btn_clear_log_clicked();
    void on_btn_export_log_clicked();

private:
    void appendLog(const FlowRunLogInfo& logInfo);
    void appendLog(QtMsgType type, const QString& log, qint64 timestamp = -1);
    void reloadDeviceLogs();
};