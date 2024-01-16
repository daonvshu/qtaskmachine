#include "targetsignaltransition.h"

#include <qstatemachine.h>
#include <qdebug.h>

bool TargetSignalTransition::eventTest(QEvent *event) {
    if (!QSignalTransition::eventTest(event)) {
        return false;
    }

    if (signalTester == nullptr) {
        return true;
    }

    auto se = dynamic_cast<QStateMachine::SignalEvent*>(event);
    return signalTester(se->arguments());
}

void TargetSignalTransition::setSignalDataHandler(const std::function<bool(const QVariantList &)> &handler) {
    signalTester = handler;
}
