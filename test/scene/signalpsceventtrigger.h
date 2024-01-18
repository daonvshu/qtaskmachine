#pragma once

#include <qobject.h>

class SignalParamsCheckEventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void trigger(int value);
};