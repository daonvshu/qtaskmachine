#pragma once

#include <qwidget.h>

#include "ui_floatingtoolbar.h"

class FloatingToolbar : public QWidget {
    Q_OBJECT
public:
    explicit FloatingToolbar(QWidget* parent = nullptr);

private slots:
    void on_btn_snap_clicked(bool checked);
    void on_btn_dir_clicked(bool checked);

private:
    Ui::FloatingToolbar ui;
};