#include "directstate.h"

DirectState::DirectState(QState *parent) : LinearState(parent) {

}

LinearState::TransitionType DirectState::getTransitionType() {
    return TransitionType::Directly;
}

TargetSignalTransition *DirectState::getSignalTransition() {
    return nullptr;
}
