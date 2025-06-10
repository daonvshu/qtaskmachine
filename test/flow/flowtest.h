#pragma once

#include <qobject.h>
#include <qpointer.h>

#include "../pageloadinterface.h"

#include "ui_flowtest.h"

#include <stateutils/taskmachinerunner.h>

class FlowTest : public PageLoadInterface {
    Q_OBJECT

    Q_PROPERTY(int testIntValue MEMBER testIntValue)

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

    int testIntValue = -1;

    Ui::FlowTest ui;
    QPointer<TaskMachineRunner> runner;

private slots:
    void onFinishedCall();
    void showDialog();

private:
    Q_INVOKABLE bool checkExitFunction(int index);
};

Q_DECLARE_METATYPE(FlowTest*)
