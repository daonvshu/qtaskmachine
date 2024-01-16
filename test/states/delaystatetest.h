#pragma once

#include <qobject.h>

#include "../pageloadinterface.h"

#include "ui_delaystatetest.h"

class DelayStateTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit DelayStateTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

private:
    static int id;

    Ui::DelayStateTest ui;
};

Q_DECLARE_METATYPE(DelayStateTest*)
