#pragma once

#include <qobject.h>
#include <qpointer.h>

#include "../pageloadinterface.h"

#include "targetlessstaterun.h"

#include "ui_targetlessstatetest.h"

class TargetLessStateTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit TargetLessStateTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

private:
    static int id;

    Ui::TargetLessStateTest ui;
    QPointer<TargetLessEventTrigger> eventTrigger;
};

Q_DECLARE_METATYPE(TargetLessStateTest*)
