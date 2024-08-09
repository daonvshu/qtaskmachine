#pragma once

#include <qwidget.h>

#include "ui_configeventstate.h"

class ConfigEventState : public QWidget {
public:
    explicit ConfigEventState(QWidget *parent = nullptr);

private:
    Ui::ConfigEventState ui;
};
