#include "directstate.h"

DirectState::DirectState(QState *parent) : LinearState(parent) {

}

void DirectState::onEntry(QEvent *event) {
    clearTransitions();
    addTransition(getTargetState());
}
