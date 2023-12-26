#pragma once

#include <qabstracttransition.h>
#include <functional>

class ConditionalTransition : public QAbstractTransition {
public:
    ConditionalTransition(QAbstractState* positiveState, QAbstractState* negativeState, QState* parent = nullptr)
        : QAbstractTransition(parent)
        , positiveState(positiveState)
        , negativeState(negativeState)
    {
    }

    std::function<bool()> conditionReader = nullptr;

protected:
    bool eventTest(QEvent *event) override {
        return true;
    }

    void onTransition(QEvent *event) override {
        Q_ASSERT(conditionReader != nullptr);
        this->setTargetState(conditionReader() ? positiveState : negativeState);
    }

private:
    QAbstractState* positiveState;
    QAbstractState* negativeState;
};