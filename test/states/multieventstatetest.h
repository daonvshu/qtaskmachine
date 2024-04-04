#pragma once

#include <qobject.h>

#include "../pageloadinterface.h"

#include "ui_multieventstatetest.h"

#include "multieventstaterun.h"

class MultiEventStateTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit MultiEventStateTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

private:
    static int id;

    Ui::MultiEventStateTest ui;

    QPointer<MultiEventTrigger> eventTrigger;
};

Q_DECLARE_METATYPE(MultiEventStateTest*)
