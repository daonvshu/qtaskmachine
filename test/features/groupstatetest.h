#pragma once

#include <qobject.h>

#include "../pageloadinterface.h"

#include "ui_groupstatetest.h"

#include "groupstaterun1.h"
#include "groupstaterun2.h"

class GroupStateTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit GroupStateTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

private:
    static int id;

    Ui::GroupStateTest ui;
    QPointer<GroupEventTrigger> eventTrigger;
};

Q_DECLARE_METATYPE(GroupStateTest*)