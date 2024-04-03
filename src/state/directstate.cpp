#include "directstate.h"

DirectState::DirectState(QState *parent) : LinearState(parent) {

}

void DirectState::onEntry(QEvent *event) {
    LinearState::onEntry(event);

    clearTransitions();

    auto target = getTargetState();
    //子状态转移
    if (hasChild) {
        addTransition(this, &QState::finished, target);
    } else {
        Q_ASSERT(target != this);
        addTransition(target);
    }
}
