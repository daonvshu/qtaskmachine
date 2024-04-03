#pragma once

#include <qstate.h>
#include <functional>
#include <qloggingcategory.h>

typedef const QLoggingCategory& (*LoggingCategoryPtr)();

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

    /**
     * @brief 设置当前状态名称，如果设置了category，则会在进入和离开时打印
     * @param name
     * @param categoryPtr
     */
    void setStateName(const QString& name, LoggingCategoryPtr categoryPtr = nullptr);

protected:
    void clearTransitions();
    QAbstractState* getTargetState();

    void onEntry(QEvent *event) override;
    void onExit(QEvent *event) override;

protected:
    struct SelectState {
        QList<QAbstractState*> targetStates;
        std::function<int()> condition = nullptr;
    };

    SelectState selectState;
    bool hasChild;

    QString stateName;
    LoggingCategoryPtr debugPtr = nullptr;
};