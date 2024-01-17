#pragma once

#include <qstate.h>
#include <functional>

class LinearState : public QState {
public:
    explicit LinearState(QState* parent = nullptr);

    /**
     * @brief 添加状态切换目标
     * @param nextState 下一个状态
     * @return 下一个状态
     */
    LinearState& operator>>(LinearState* nextState);
    LinearState& operator>>(QAbstractState* nextState);
    LinearState& target(LinearState* nextState);
    LinearState& target(QAbstractState* nextState);

    /**
     * @brief 添加条件状态切换目标
     * @param conditionState 条件状态
     * @return 本身状态
     */
    LinearState& operator<<(QAbstractState* conditionState);
    LinearState& next(QAbstractState* conditionState);

    void setCondition(int condition);
    void setCondition(const std::function<int()>& condition);

protected:
    void clearTransitions();
    QAbstractState* getTargetState();

protected:
    struct SelectState {
        QList<QAbstractState*> targetStates;
        std::function<int()> condition = nullptr;
    };

    SelectState selectState;
    bool hasChild;
};