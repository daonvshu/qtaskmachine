#pragma once

#include <qstate.h>
#include <functional>

class LinearState : public QState {
public:
    explicit LinearState(QState* parent = nullptr);

    LinearState& operator>>(LinearState* nextState);

    void operator>>(QAbstractState* nextState);

    void setNext(QAbstractState* positive, QAbstractState* negative);

    void setCondition(const std::function<bool()>& condition);

    void setInstantCondition(bool condition);

protected:
    void clearTransitions();
    QAbstractState* getTargetState();

protected:
    struct SelectState {
        QAbstractState* positive = nullptr;
        QAbstractState* negative = nullptr;

        std::function<bool()> condition = nullptr;
    };

    SelectState selectState;
};