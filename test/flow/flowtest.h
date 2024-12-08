#pragma once

#include <qobject.h>
#include <qpointer.h>

#include "../pageloadinterface.h"

#include "ui_flowtest.h"

#include <stateutils/taskmachinerunner.h>

class FlowTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit FlowTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

signals:
    void testEvent1();

private:
    static int id;

    Ui::FlowTest ui;
    QPointer<TaskMachineRunner> runner;
};

Q_DECLARE_METATYPE(FlowTest*)
