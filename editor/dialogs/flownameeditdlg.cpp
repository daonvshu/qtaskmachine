#include "flownameeditdlg.h"

#include <QWKWidgets/widgetwindowagent.h>

FlowNameEditDlg::FlowNameEditDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
    agent->setHitTestVisible(ui.system_buttons);
}

void FlowNameEditDlg::on_btn_close_clicked() {
    reject();
}

void FlowNameEditDlg::on_btn_cancel_clicked() {
    reject();
}

void FlowNameEditDlg::on_btn_confirm_clicked() {
    accept();
}

QString FlowNameEditDlg::showDialog(const QString &oldName, const QString &title, QWidget *parent) {
    FlowNameEditDlg dlg(parent);
    dlg.ui.title->setText(title);
    dlg.ui.input_name->setText(oldName);
    if (dlg.exec() == QDialog::Accepted) {
        return dlg.ui.input_name->text();
    } else {
        return oldName;
    }
}
