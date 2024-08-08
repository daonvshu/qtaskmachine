#pragma once

#include <qobject.h>
#include <qpointer.h>

#include "flowchart/elements/fcexecutoritem.h"

namespace Ui {
    class App;
}

class CommonPropManager : public QObject {
public:
    explicit CommonPropManager(Ui::App* appUi, QObject *parent = nullptr);

    void loadItemBaseData(FcExecutorItem* item);

private slots:
    void nameEditFinished();
    void funcEnterEditFinished();
    void funcExitEditFinished();

private:
    Ui::App* appUi;
    FcExecutorItem* activeItem = nullptr;
};
