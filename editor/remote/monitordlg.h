#pragma once

#include <qdialog.h>

#include "ui_monitordlg.h"

class MonitorDlg : public QDialog {
    Q_OBJECT

public:
    explicit MonitorDlg(QWidget *parent = nullptr);

private:
    Ui::MonitorDlg ui;

private slots:
    void on_btn_close_clicked();
    void on_btn_confirm_clicked();
    void on_btn_connect_clicked();
    void on_btn_clear_log_clicked();
    void on_btn_export_log_clicked();
};