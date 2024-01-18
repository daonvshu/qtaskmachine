#pragma once

#include <qobject.h>

#include "../pageloadinterface.h"

#include "ui_signalparamschecktest.h"

#include "signalpsceventtrigger.h"

class SignalParamsCheckTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit SignalParamsCheckTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

private:
    static int id;

    Ui::SignalParamsCheckTest ui;

    QPointer<SignalParamsCheckEventTrigger> eventTrigger;

private:
    int getType() const;
};

Q_DECLARE_METATYPE(SignalParamsCheckTest*)
