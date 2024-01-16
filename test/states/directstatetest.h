#pragma once

#include <qwidget.h>
#include <qpointer.h>
#include <qstatemachine.h>

#include "../pageloadinterface.h"

class DirectStateTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit DirectStateTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget * getExtraInputWidgets() override;

private:
    static int id;

    QPointer<QStateMachine> stateMachine;
};

Q_DECLARE_METATYPE(DirectStateTest*)