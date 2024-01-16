#include "linearstate.h"

#include "targetsignaltransition.h"

#include <qdebug.h>

LinearState::LinearState(QState *parent)
    : QState(parent)
{}

LinearState &LinearState::operator>>(LinearState *nextState) {
    selectState.positive = nextState;
    return *nextState;
}

void LinearState::operator>>(QAbstractState *nextState) {
    selectState.positive = nextState;
}

void LinearState::setNext(QAbstractState *positive, QAbstractState *negative) {
    selectState.positive = positive;
    selectState.negative = negative;
}

void LinearState::setCondition(const std::function<bool()> &condition) {
    selectState.condition = condition;
}

void LinearState::setInstantCondition(bool condition) {
    setCondition([=] {
        return condition;
    });
}

void LinearState::clearTransitions() {
    auto oldTransitions = transitions();
    for (auto tr : oldTransitions) {
        removeTransition(tr);
    }
}

QAbstractState *LinearState::getTargetState() {
    if (selectState.condition != nullptr) {
        return selectState.condition() ? selectState.positive : selectState.negative;
    } else {
        return selectState.positive;
    }
}
