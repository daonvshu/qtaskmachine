#include "messagedlg.h"

#include <QWKWidgets/widgetwindowagent.h>

MessageDlg::MessageDlg(QWidget *parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    ui.setupUi(this);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
    agent->setHitTestVisible(ui.system_buttons);
}

MessageDlg* MessageDlg::showMessage(const QString &title, const QString &message, QWidget *parent) {
    auto dlg = new MessageDlg(parent);
    dlg->ui.title->setText(title);
    dlg->ui.label_content->setText(message);
    dlg->show();
    return dlg;
}

void MessageDlg::on_btn_close_clicked() {
    reject();
}

void MessageDlg::on_btn_confirm_clicked() {
    reject();
}
