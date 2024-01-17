#pragma once

#include <qobject.h>

#include "../pageloadinterface.h"

#include "ui_conditionalstatetest.h"

class ConditionalStateTest : public PageLoadInterface {
    Q_OBJECT

public:
    Q_INVOKABLE explicit ConditionalStateTest(QObject *parent = nullptr);

    QString getDescription() override;

    QString getCodeFile() override;

    void run() override;

    QWidget *getExtraInputWidgets() override;

private:
    static int id;

    Ui::ConditionalStateTest ui;

private:
    int getTypeIndex() const;
};

Q_DECLARE_METATYPE(ConditionalStateTest*)
