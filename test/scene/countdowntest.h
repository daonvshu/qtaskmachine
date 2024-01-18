#pragma once

#include <qobject.h>

#include "../pageloadinterface.h"

#include "ui_countdowntest.h"

#include "countdownrun.h"

class CountDownTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit CountDownTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

private:
    static int id;

    Ui::CountDownTest ui;

    QPointer<CountDownEventTrigger> eventTrigger;
};

Q_DECLARE_METATYPE(CountDownTest*)
