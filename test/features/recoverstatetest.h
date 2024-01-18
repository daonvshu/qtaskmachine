#pragma once

#include <qobject.h>

#include "../pageloadinterface.h"

#include "ui_recoverstatetest.h"

#include "recoverstaterun.h"

class RecoverStateTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit RecoverStateTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

private:
    static int id;

    Ui::RecoverStateTest ui;
    QPointer<RecoverEventTrigger> eventTrigger;
};

Q_DECLARE_METATYPE(RecoverStateTest*)
