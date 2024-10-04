#pragma once

#include <qwidget.h>

#include "ui_app.h"

class App : public QWidget {
    Q_OBJECT

public:
    explicit App(QWidget *parent = nullptr);

private:
    Ui::App ui;

private slots:
    void on_btn_min_clicked();
    void on_btn_max_clicked();
    void on_btn_close_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;
};
