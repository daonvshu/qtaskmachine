#pragma once

#include <qwidget.h>
#include <functional>

#include <qpointer.h>
#include <qstatemachine.h>

class PageLoadInterface : public QObject {
public:
    using QObject::QObject;

    virtual QString getDescription() = 0;
    virtual QString getCodeFile() = 0;
    virtual void run() = 0;

    virtual QWidget* getExtraInputWidgets() = 0;

    std::function<void()> requestCodeReload = nullptr;

protected:
    QPointer<QStateMachine> stateMachine;
};