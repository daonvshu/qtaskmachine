#include "directstate.h"

DirectState::DirectState(QState *parent) : LinearState(parent) {

}

void DirectState::onEntry(QEvent *event) {
    clearTransitions();

    //子状态转移
    if (hasChild) {
        addTransition(this, &QState::finished, getTargetState());
    } else {
        addTransition(getTargetState());
    }
}
