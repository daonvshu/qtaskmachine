#pragma once

#include <qobject.h>
#include <qstatemachine.h>

#include "configflows.h"

#include "state/eventstate.h"
#include "state/multieventstate.h"

class TaskMachineRunner : public QObject {
    Q_OBJECT

public:
    explicit TaskMachineRunner(const QString& flowName, QObject *parent = nullptr);
    ~TaskMachineRunner();

    TaskMachine::ConfigFlow& getCurrentFlow();

    bool run(QObject* context);

    void setLogging(LoggingCategoryPtr categoryPtr);

    void cancel();

signals:
    void finished();

private:
    TaskMachine::ConfigFlow configFlow;
    QObject* currentBindContext = nullptr;
    QStateMachine* currentStateMachine = nullptr;
    LoggingCategoryPtr debugPtr = nullptr;

    //tmp data
    QHash<int, const TaskMachine::ConfigFlowExecutor*> executors;
    QMultiHash<int, const TaskMachine::ConfigFlowConnectLine*> fromLines, toLines;
    QHash<int, QAbstractState*> createdState;

private:
    void stepStateMachine();

    QAbstractState* createStateByType(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createDirectState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createDelayState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createEventState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createMultiEventState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createConditionState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createGroupState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createHistoryState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createEndState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);

    QAbstractState* findGroupParent(const TaskMachine::ConfigFlowExecutor* executor);
    void bindExecutorBaseInfo(QAbstractState* state, const TaskMachine::ConfigFlowExecutor* executor);

    void invokeFunction(const QString& slotName);
    QMetaMethod findFunction(const QString& signalName);

    void printLog(QtMsgType msgType, const QString& message);

    friend class BindableCheckEventState;
    friend class BindableMultiCheckEventState;
};

class BindableCheckEventState : public EventState {
    Q_OBJECT

public:
    using EventState::EventState;

    void setCheckFunction(QObject* context, const QMetaMethod& function, const QString& funcName) {
        checkFunction = function;
        functionName = funcName;
        currentContext = context;
    }

signals:
    void checkFunctionInvokeFailed(const QString& funcName);

protected:
    bool testFinishBySignalData(const QVariantList &data) override;

private:
    QMetaMethod checkFunction;
    QString functionName;
    QObject* currentContext;
};

class BindableMultiCheckEventState : public MultiEventState {
    Q_OBJECT

public:
    using MultiEventState::MultiEventState;

    void setCheckFunction(QObject* context, int signalIndex, int branchId, const QMetaMethod& function, const QString& funcName) {
        checkFunction[signalIndex] = qMakePair(branchId, function);
        functionNames[signalIndex] = funcName;
        currentContext = context;
    }

signals:
    void checkFunctionInvokeFailed(const QString& funcName);

protected:
    bool testFinishBySignalData(int signalIndex, const QVariantList &data) override;

private:
    QHash<int, QPair<int, QMetaMethod>> checkFunction;
    QHash<int, QString> functionNames;
    QObject* currentContext;
};