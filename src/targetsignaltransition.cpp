#include "targetsignaltransition.h"

#include <qstatemachine.h>
#include <qdebug.h>

bool TargetSignalTransition::eventTest(QEvent *event) {
    if (!QSignalTransition::eventTest(event)) {
        return false;
    }
    auto se = dynamic_cast<QStateMachine::SignalEvent*>(event);
    emit signalTriggered(se->arguments());

    return --signalTriggeredSize <= 0;
}

void TargetSignalTransition::setSignalTriggeredSize(int size) {
    signalTriggeredSize = size;
}
