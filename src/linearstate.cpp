#include "linearstate.h"

#include "targetsignaltransition.h"

LinearState::LinearState(QState *parent)
    : QState(parent)
    , hasChild(false)
{
    if (auto parentLinear = dynamic_cast<LinearState*>(parent)) {
        parentLinear->hasChild = true;
    }
}

LinearState &LinearState::operator>>(LinearState *nextState) {
    return target(nextState);
}

LinearState& LinearState::operator>>(QAbstractState *nextState) {
    return target(nextState);
}

LinearState &LinearState::target(LinearState *nextState) {
    target((QAbstractState*)nextState);
    return *nextState;
}

LinearState &LinearState::target(QAbstractState *nextState) {
    if (selectState.targetStates.isEmpty()) {
        selectState.targetStates << nextState;
    } else {
        for (auto target : selectState.targetStates) {
            if (target == nextState) {
                continue;
            }
            if (auto linearTag = dynamic_cast<LinearState*>(target)) {
                linearTag->target(nextState);
            } else if (auto state = dynamic_cast<QState*>(target)) {
                state->addTransition(nextState);
            }
        }
    }
    return *this;
}

LinearState &LinearState::operator<<(QAbstractState* conditionState) {
    return next(conditionState);
}

LinearState &LinearState::next(QAbstractState *conditionState) {
    selectState.targetStates << conditionState;
    return *this;
}

void LinearState::setCondition(int condition) {
    setCondition([=] {
        return condition;
    });
}

void LinearState::setCondition(const std::function<int()> &condition) {
    selectState.condition = condition;
}

void LinearState::setStateName(const QString &name, LoggingCategoryPtr categoryPtr) {
    stateName = name;
    debugPtr = categoryPtr;
}

void LinearState::clearTransitions() {
    auto oldTransitions = transitions();
    for (auto tr : oldTransitions) {
        removeTransition(tr);
    }
}

QAbstractState *LinearState::getTargetState() {
    if (selectState.condition != nullptr) {
        int targetIndex = selectState.condition();
        if (targetIndex >= 0 && targetIndex < selectState.targetStates.size()) {
            return selectState.targetStates[targetIndex];
        }
    }
    if (selectState.targetStates.isEmpty()) {
        return this;
    }
    return selectState.targetStates[0];
}

void LinearState::onEntry(QEvent *event) {
    QState::onEntry(event);
    if (debugPtr && !stateName.isEmpty()) {
        qCInfo(debugPtr).nospace() << "state '" << stateName << "' enter!";
    }
}

void LinearState::onExit(QEvent *event) {
    QState::onExit(event);
    if (debugPtr && !stateName.isEmpty()) {
        qCInfo(debugPtr).nospace() << "state '" << stateName << "' exit!";
    }
}
