#pragma once

#include <qobject.h>

#include "ui_flownameeditdlg.h"

class FlowNameEditDlg : public QDialog {
    Q_OBJECT

public:
    explicit FlowNameEditDlg(QWidget *parent = nullptr);

    static QString showDialog(const QString& oldName, const QString& title, QWidget* parent = nullptr);

private:
    Ui::FlowNameEditDlg ui;

protected slots:
    void on_btn_close_clicked();
    void on_btn_cancel_clicked();
    void on_btn_confirm_clicked();
};
