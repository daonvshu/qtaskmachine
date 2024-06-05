#pragma once

#include <qstate.h>
#include <functional>
#include <qloggingcategory.h>

typedef const QLoggingCategory& (*LoggingCategoryPtr)();

enum class TaskStateType {
    State_Enter,
    State_Exit,
};

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

    /**
     * @brief 延迟条件切换目标，等同于在onExit时检查条件并切换目标，需要在设置切换和条件前调用
     */
    void setConditionDeferrable();

    template<typename T>
    void bindState(TaskStateType stateType, T* context, void(T::*callback)());

    template<typename T>
    void bindState(TaskStateType stateType, T* context, void(T::*callback)() const);

    template<typename T>
    void bindState(TaskStateType stateType, T* context, const std::function<void()>& callback);

    void bindState(TaskStateType stateType, const std::function<void()>& callback);

protected:
    void clearTransitions();
    QAbstractState* getTargetState();

    void onEntry(QEvent *event) override;
    void onExit(QEvent *event) override;

    static void appendTarget(QList<QAbstractState*>& group, QAbstractState* nextState);

protected:
    struct SelectState {
        QList<QAbstractState*> targetStates;
        std::function<int()> condition = nullptr;
    };

    SelectState selectState;
    bool hasChild;

    QString stateName;
    LoggingCategoryPtr debugPtr = nullptr;

    LinearState* conditionalState = nullptr;
};

template<typename T>
inline void LinearState::bindState(TaskStateType stateType, T *context, void(T::*callback)()) {
    bindState(stateType, context, [=] {
        (context->*callback)();
    });
}

template<typename T>
inline void LinearState::bindState(TaskStateType stateType, T *context, void(T::*callback)() const) {
    bindState(stateType, context, [=] {
        (context->*callback)();
    });
}

template<typename T>
inline void LinearState::bindState(TaskStateType stateType, T *context, const std::function<void()> &callback)  {
    switch (stateType) {
        case TaskStateType::State_Enter:
            connect(this, &QState::entered, context, callback);
            break;
        case TaskStateType::State_Exit:
            connect(this, &QState::exited, context, callback);
            break;
    }
}

inline void LinearState::bindState(TaskStateType stateType, const std::function<void()> &callback) {
    switch (stateType) {
        case TaskStateType::State_Enter:
            connect(this, &QState::entered, callback);
            break;
        case TaskStateType::State_Exit:
            connect(this, &QState::exited, callback);
            break;
    }
}