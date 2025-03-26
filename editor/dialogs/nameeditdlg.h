#pragma once

#include <qobject.h>

#include "ui_nameeditdlg.h"

class NameEditDlg : public QDialog {
    Q_OBJECT

public:
    explicit NameEditDlg(QWidget *parent = nullptr);

    static QString showDialog(const QString& oldName, const QString& title, const QString& inputLabel, QWidget* parent = nullptr);

private:
    Ui::NameEditDlg ui;

protected slots:
    void on_btn_close_clicked();
    void on_btn_cancel_clicked();
    void on_btn_confirm_clicked();
};
