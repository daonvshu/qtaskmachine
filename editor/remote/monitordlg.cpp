#include "monitordlg.h"

#include <QWKWidgets/widgetwindowagent.h>

MonitorDlg::MonitorDlg(QWidget *parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    ui.setupUi(this);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
    agent->setHitTestVisible(ui.system_buttons);
}

void MonitorDlg::on_btn_close_clicked() {
    reject();
}

void MonitorDlg::on_btn_confirm_clicked() {
    accept();
}

void MonitorDlg::on_btn_connect_clicked() {
}

void MonitorDlg::on_btn_clear_log_clicked() {
}

void MonitorDlg::on_btn_export_log_clicked() {
}
