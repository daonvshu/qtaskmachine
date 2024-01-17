#pragma once

#include <qobject.h>

#include "../pageloadinterface.h"

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
};

Q_DECLARE_METATYPE(GroupStateTest*)