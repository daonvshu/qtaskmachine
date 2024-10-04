#pragma once

#include <qwidget.h>

#include "ui_app.h"

class App : public QWidget {
public:
    explicit App(QWidget *parent = nullptr);

private:
    Ui::App ui;
};
