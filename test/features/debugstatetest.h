#pragma once

#include <qobject.h>

#include "../pageloadinterface.h"

#include "debugstaterun.h"

class DebugStateTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit DebugStateTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

private:
    static int id;
};

Q_DECLARE_METATYPE(DebugStateTest*)