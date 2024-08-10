#pragma once

#include <qobject.h>
#include <qpointer.h>

#include "flowchart/elements/fcexecutoritem.h"
#include "flowchart/elements/fcbranchitem.h"

namespace Ui {
    class App;
}

class CommonPropManager : public QObject {
    Q_OBJECT

public:
    explicit CommonPropManager(Ui::App* appUi, QObject *parent = nullptr);

    void loadItemBaseData(FcExecutorItem* item);
    void loadItemBaseData(FcConditionalItem* item);

signals:
    void nameChanged();

private slots:
    void nameEditFinished();
    void funcEnterEditFinished();
    void funcExitEditFinished();

private:
    Ui::App* appUi;
    FcExecutorItem* activeItem = nullptr;
    FcConditionalItem* activeConditionalItem = nullptr;
};
