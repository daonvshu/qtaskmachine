#pragma once

#include <qdialog.h>

#include "ui_messagedlg.h"

class MessageDlg : public QDialog {
    Q_OBJECT

public:
    explicit MessageDlg(QWidget *parent = nullptr);

    static MessageDlg* showMessage(const QString& title, const QString& message, QWidget* parent = nullptr, bool cancelAble = false);

private:
    Ui::MessageDlg ui;

private slots:
    void on_btn_close_clicked();
    void on_btn_confirm_clicked();
    void on_btn_cancel_clicked();
};
