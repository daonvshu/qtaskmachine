#pragma once

#include <qobject.h>

#include "../pageloadinterface.h"

#include "ui_eventstatetest.h"

#include "eventstaterun.h"

class EventStateTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit EventStateTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

private:
    static int id;

    Ui::EventStateTest ui;

    QPointer<EventTrigger> eventTrigger;
};

Q_DECLARE_METATYPE(EventStateTest*)
