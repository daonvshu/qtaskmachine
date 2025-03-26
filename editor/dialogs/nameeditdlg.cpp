#include "nameeditdlg.h"

#include <QWKWidgets/widgetwindowagent.h>

NameEditDlg::NameEditDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
    agent->setHitTestVisible(ui.system_buttons);
}

void NameEditDlg::on_btn_close_clicked() {
    reject();
}

void NameEditDlg::on_btn_cancel_clicked() {
    reject();
}

void NameEditDlg::on_btn_confirm_clicked() {
    if (ui.input_name->text().isEmpty()) {
        return;
    }
    accept();
}

QString NameEditDlg::showDialog(const QString &oldName, const QString &title, const QString& inputLabel, QWidget *parent) {
    NameEditDlg dlg(parent);
    dlg.ui.title->setText(title);
    dlg.ui.input_name->setText(oldName);
    dlg.ui.label_name->setText(inputLabel);
    if (dlg.exec() == Accepted) {
        return dlg.ui.input_name->text();
    }
    return {};
}
