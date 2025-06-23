#include "taskmachinerunner.h"

#include "taskmachinesteputil.h"

#include "state/directstate.h"
#include "state/delaystate.h"
#include "state/eventstate.h"
#include "state/groupstate.h"

#ifdef QTASK_MACHINE_REMOTE_DEBUG_ENABLED
#include "remote/remotedebuglistener.h"
#endif

#include <qfinalstate.h>
#include <qhistorystate.h>

#include <qloggingcategory.h>
#include <qdebug.h>
#include <qdatetime.h>

using namespace TaskMachine;
TaskMachineRunner::TaskMachineRunner(const QString& flowName, QObject *parent)
    : QObject(parent)
{
    configFlow = TaskMachineStepUtil::getConfigFlow(flowName);
}

TaskMachineRunner::~TaskMachineRunner() {
    if (currentStateMachine && currentStateMachine->isRunning()) {
#ifdef QTASK_MACHINE_REMOTE_DEBUG_ENABLED
        RemoteDebugListener::instance().flowFinished(configFlow.name());
#endif
    }
}

TaskMachine::ConfigFlow &TaskMachineRunner::getCurrentFlow() {
    return configFlow;
}

bool TaskMachineRunner::run(QObject *context) {
    currentBindContext = context;
    stepStateMachine();

    if (currentStateMachine) {
        currentStateMachine->start();
#ifdef QTASK_MACHINE_REMOTE_DEBUG_ENABLED
        RemoteDebugListener::instance().flowBegin(configFlow.name());
#endif
    }

    return currentStateMachine != nullptr;
}

void TaskMachineRunner::setLogging(LoggingCategoryPtr categoryPtr) {
    debugPtr = categoryPtr;
}

void TaskMachineRunner::cancel() {
    delete currentStateMachine;
    currentStateMachine = nullptr;
}

void TaskMachineRunner::stepStateMachine() {
    delete currentStateMachine;
    currentStateMachine = nullptr;

    executors.clear();
    fromLines.clear();
    toLines.clear();
    createdState.clear();

    const TaskMachine::ConfigFlowExecutor* beginExecutor = nullptr;
    for (const ConfigFlowExecutor& executor : configFlow.executors()) {
        executors[executor.id()] = &executor;
        if (FlowChartNodeType(executor.type()) == FlowChartNodeType::Node_Begin) {
            beginExecutor = &executor;
        }
    }

    if (beginExecutor == nullptr) {
        return;
    }

    for (const ConfigFlowConnectLine& line : configFlow.lines()) {
        fromLines.insert(line.connectFrom(), &line);
        toLines.insert(line.connectTo(), &line);
    }

    currentStateMachine = new QStateMachine(this);
    connect(currentStateMachine, &QStateMachine::finished, this, [&] {
#ifdef QTASK_MACHINE_REMOTE_DEBUG_ENABLED
        RemoteDebugListener::instance().flowFinished(configFlow.name());
#endif
        finished();
    });
    auto beginState = createStateByType(beginExecutor, currentStateMachine);
    currentStateMachine->setInitialState(beginState);
}

QAbstractState* TaskMachineRunner::createStateByType(const TaskMachine::ConfigFlowExecutor *executor, QState* parent) {
    if (createdState.contains(executor->id())) {
        return createdState[executor->id()];
    }
    switch (FlowChartNodeType(executor->type())) {
        case FlowChartNodeType::Node_Begin:
        case FlowChartNodeType::Node_Normal:
            return createDirectState(executor, parent);
        case FlowChartNodeType::Node_Delay:
            return createDelayState(executor, parent);
        case FlowChartNodeType::Node_Event:
            return createEventState(executor, parent);
        case FlowChartNodeType::Node_MultiEvent:
            return createMultiEventState(executor, parent);
        case FlowChartNodeType::Node_Condition:
            return createConditionState(executor, parent);
        case FlowChartNodeType::Node_Group:
            return createGroupState(executor, parent);
        case FlowChartNodeType::Node_History:
            return createHistoryState(executor, parent);
        case FlowChartNodeType::Node_End:
            return createEndState(executor, parent);
        case FlowChartNodeType::Node_Loop:
            return createLoopState(executor, parent);
    }
    return nullptr;
}

QAbstractState* TaskMachineRunner::createDirectState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent) {
    auto state = new DirectState(parent);
    createdState[executor->id()] = state;
    bindExecutorBaseInfo(state, executor);

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }
    auto connectLine = connectLines.first();
    auto nextExecutor = executors[connectLine->connectTo()];
    if (nextExecutor == nullptr) {
        return state;
    }
    auto nextState = createStateByType(nextExecutor, parent);
    state->next(nextState);
    return state;
}

QAbstractState* TaskMachineRunner::createDelayState(const TaskMachine::ConfigFlowExecutor *executor, QState* parent) {
    int delayMs = executor->delay();
    auto delayProp = executor->delayProperty();
    if (!delayProp.isEmpty()) {
        delayMs = currentBindContext->property(delayProp.toLatin1()).toInt();
    }

    auto state = new DelayState(delayMs, parent);
    createdState[executor->id()] = state;
    bindExecutorBaseInfo(state, executor);

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }
    auto connectLine = connectLines.first();
    auto nextExecutor = executors[connectLine->connectTo()];
    if (nextExecutor == nullptr) {
        return state;
    }
    auto nextState = createStateByType(nextExecutor, parent);
    state->next(nextState);
    return state;
}

QAbstractState* TaskMachineRunner::createEventState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new BindableCheckEventState(parent);
    connect(state, &BindableCheckEventState::checkFunctionInvokeFailed, [&] (const QString& funcName) {
        printLog(QtMsgType::QtCriticalMsg, "invoke check function fail:" + funcName);
    });

    createdState[executor->id()] = state;
    bindExecutorBaseInfo(state, executor);

    state->setTimeout(executor->timeout());
    state->setRetrySize(executor->retry());

    if (!executor->funcRetry().isEmpty()) {
        connect(state, &EventState::stateRetry, [this, executor] {
            invokeFunction(executor->funcRetry());
        });
    }

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }

    for (const auto connectLine : connectLines) {
        auto nextExecutor = executors[connectLine->connectTo()];
        if (nextExecutor == nullptr) {
            continue;
        }
        auto nextState = createStateByType(nextExecutor, parent);
        auto triggerFunc = findFunction(connectLine->trigger());
        if (!connectLine->failBranch()) {
            if (triggerFunc.isValid()) {
                state->setSignal(currentBindContext, triggerFunc.methodSignature().constData());
            }
            state->setCheckFunction(currentBindContext, findFunction(connectLine->checkFunc()), connectLine->checkFunc());
            state->next(nextState);
        } else {
            if (triggerFunc.isValid()) {
                connect(currentBindContext, triggerFunc, state, QMetaMethod::fromSignal(&EventState::triggerSignalFail));
            }
            state->setFailState(nextState);
        }
    }
    return state;
}

QAbstractState *TaskMachineRunner::createMultiEventState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new BindableMultiCheckEventState(parent);
    connect(state, &BindableMultiCheckEventState::checkFunctionInvokeFailed, [&] (const QString& funcName) {
        printLog(QtMsgType::QtCriticalMsg, "invoke check function fail:" + funcName);
    });

    createdState[executor->id()] = state;
    bindExecutorBaseInfo(state, executor);

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }

    int signalIndex = 0;
    for (const auto connectLine : connectLines) {
        auto nextExecutor = executors[connectLine->connectTo()];
        if (nextExecutor == nullptr) {
            continue;
        }
        auto nextState = createStateByType(nextExecutor, parent);
        auto triggerFunc = findFunction(connectLine->trigger());
        if (triggerFunc.isValid()) {
            state->addSignal(currentBindContext, triggerFunc.methodSignature().constData());
            state->next(nextState);
            state->setCheckFunction(currentBindContext, signalIndex++, connectLine->branchId(), findFunction(connectLine->checkFunc()), connectLine->checkFunc());
        }
    }
    return state;
}

QAbstractState *TaskMachineRunner::createConditionState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new DirectState(parent);
    createdState[executor->id()] = state;
    bindExecutorBaseInfo(state, executor);

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }
    int signalIndex = 0;
    QHash<int, int> idToIndexMap;
    QHash<int, QString> idToNameMap;
    for (const auto connectLine : connectLines) {
        auto nextExecutor = executors[connectLine->connectTo()];
        if (nextExecutor == nullptr) {
            continue;
        }
        auto nextState = createStateByType(nextExecutor, parent);
        state->next(nextState);
        idToIndexMap[connectLine->branchId()] = signalIndex++;
        idToNameMap[connectLine->branchId()] = connectLine->branchName();
    }
    state->setCondition([=] {
        int branchId = 0;
        if (executor->condition().isEmpty()) {
            printLog(QtMsgType::QtCriticalMsg, "condition check function is empty!");
            return 0;
        }
        if (executor->condition().endsWith("()")) {
            auto checkFunc = findFunction(executor->condition());
            auto returnValue = Q_RETURN_ARG(int, branchId);
            bool invokeResult = checkFunc.invoke(currentBindContext, Qt::DirectConnection, returnValue);
            if (!invokeResult) {
                printLog(QtMsgType::QtCriticalMsg, "invoke check function fail:" + executor->condition());
                return 0;
            }
        } else {
            auto propertyData = currentBindContext->property(executor->condition().toLatin1());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            switch (propertyData.typeId()) {
                case QMetaType::Int:
                    branchId = propertyData.toInt();
                    break;
                case QMetaType::Bool:
                    branchId = propertyData.toBool() ? 1 : 0;
                    break;
                case QMetaType::QString:
                    branchId = idToNameMap.key(propertyData.toString());
                    break;
#else
            switch (propertyData.type()) {
                case QVariant::Int:
                    branchId = propertyData.toInt();
                    break;
                case QVariant::Bool:
                    branchId = propertyData.toBool() ? 1 : 0;
                    break;
                case QVariant::String:
                    branchId = idToNameMap.key(propertyData.toString());
                    break;
#endif
                default:
                    printLog(QtMsgType::QtCriticalMsg, "condition property type not supported:" + executor->condition());
                    return 0;
            }
        }
        auto selectName = idToNameMap.value(branchId);
        if (!selectName.isEmpty()) {
            printLog(QtMsgType::QtInfoMsg, QString("select branch: %1 (%2)").arg(selectName).arg(branchId));
        } else {
            printLog(QtMsgType::QtInfoMsg, QString("select branch: %1").arg(branchId));
        }
        return idToIndexMap[branchId];
    });

    return state;
}

QAbstractState *TaskMachineRunner::createGroupState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new EventState(parent);
    createdState[executor->id()] = state;
    bindExecutorBaseInfo(state, executor);

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }

    std::sort(connectLines.begin(), connectLines.end(), [](const TaskMachine::ConfigFlowConnectLine* a, const TaskMachine::ConfigFlowConnectLine* b) {
        int flagA =  (((int)a->subBranch()) << 1) | (((int)a->failBranch()));
        int flagB = (((int)b->subBranch()) << 1) | (((int)b->failBranch()));
        return flagA > flagB;
    });

    for (const auto connectLine : connectLines) {
        auto nextExecutor = executors[connectLine->connectTo()];
        if (nextExecutor == nullptr) {
            continue;
        }

        if (connectLine->failBranch()) {
            auto nextState = createStateByType(nextExecutor, parent);
            auto triggerFunc = findFunction(connectLine->trigger());
            if (triggerFunc.isValid()) {
                connect(currentBindContext, triggerFunc, state, QMetaMethod::fromSignal(&EventState::triggerSignalFail));
            }
            state->setFailState(nextState);
        } else if (connectLine->subBranch()) {
            auto nextState = createStateByType(nextExecutor, state);
            state->setInitialState(nextState);
        } else {
            auto nextState = createStateByType(nextExecutor, parent);
            state->next(nextState);
        }
    }
    return state;
}

QAbstractState *TaskMachineRunner::createHistoryState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {

    //find parent group state
    auto groupParent = findGroupParent(executor);
    auto state = new QHistoryState(dynamic_cast<QState*>(groupParent));
    createdState[executor->id()] = state;
    bindExecutorBaseInfo(state, executor);

    //bind default state
    auto parentId = createdState.key(groupParent);
    auto fromBranch = fromLines.values(parentId);
    int defaultBranchId = -1;
    for (const auto& branch : fromBranch) {
        if (branch->subBranch()) {
            defaultBranchId = branch->connectTo();
            break;
        }
    }
    if (defaultBranchId != -1) {
        auto defaultState = createdState.value(defaultBranchId);
        state->setDefaultState(defaultState);
    }

    if (executor->nested()) {
        state->setHistoryType(QHistoryState::DeepHistory);
    }

    return state;
}

QAbstractState *TaskMachineRunner::createEndState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new QFinalState(parent);
    createdState[executor->id()] = state;
    bindExecutorBaseInfo(state, executor);

    return state;
}

QAbstractState* TaskMachineRunner::createLoopState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent) {
    auto conditionState = new DirectState(parent);
    createdState[executor->id()] = conditionState;
    bindExecutorBaseInfo(conditionState, executor);

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return conditionState;
    }

    conditionState->setCondition([=] {
        auto loopCountStr = executor->total();
        int loopCount = -1;
        if (!loopCountStr.isEmpty()) {
            bool ok;
            loopCount = loopCountStr.toInt(&ok);
            if (!ok) {
                //read property
                loopCount = currentBindContext->property(loopCountStr.toLatin1()).toInt();
            }
        }
        auto currentIndex = conditionState->property("index").toInt();

        bool exit = false;
        auto checkFunction = executor->condition();
        if (!checkFunction.isEmpty()) {
            auto checkFunc = findFunction(checkFunction);
            auto returnValue = Q_RETURN_ARG(bool, exit);
            bool invokeResult = checkFunc.invoke(currentBindContext, Qt::DirectConnection, returnValue, Q_ARG(int, currentIndex));
            if (!invokeResult) {
                printLog(QtMsgType::QtCriticalMsg, "invoke exit check function fail:" + executor->condition());
                exit = false;
            }
        }

        if (!exit) {
            if (loopCount >= 0) {
                if (currentIndex >= loopCount) {
                    exit = true;
                }
            }
        }

        if (exit) {
            currentIndex = 0;
        } else {
            currentIndex++;
        }
        conditionState->setProperty("index", currentIndex);

        return exit ? 0 : 1;
    });

    auto groupState = new GroupState(parent);
    std::sort(connectLines.begin(), connectLines.end(), [](const TaskMachine::ConfigFlowConnectLine* a, const TaskMachine::ConfigFlowConnectLine* b) {
        int flagA =  (((int)a->subBranch()) << 1) | (((int)a->failBranch()));
        int flagB = (((int)b->subBranch()) << 1) | (((int)b->failBranch()));
        return flagA > flagB;
    });

    for (const auto connectLine : connectLines) {
        auto nextExecutor = executors[connectLine->connectTo()];
        if (nextExecutor == nullptr) {
            continue;
        }

        if (connectLine->subBranch()) {
            auto nextState = createStateByType(nextExecutor, groupState);
            groupState->setInitialState(nextState);
        } else {
            auto nextState = createStateByType(nextExecutor, parent);
            *conditionState << nextState;
        }
    }

    /*
     * condition state -> (0) exit state -> ...
     *                 -> (1) group state -> condition state
     */
    *conditionState << groupState;
    *groupState >> conditionState;

    return conditionState;
}

QAbstractState *TaskMachineRunner::findGroupParent(const TaskMachine::ConfigFlowExecutor *executor) {
    auto connectToLines = toLines.values(executor->id());
    for (const auto connectLine : connectToLines) {
        auto connectFromId = connectLine->connectFrom();
        if (createdState.contains(connectFromId)) {
            auto stateExecutor = executors[connectFromId];
            if (stateExecutor->itemType() == TaskMachine::FlowChartNodeType::Node_Group) {
                return createdState.value(connectFromId);
            }
            auto parent = findGroupParent(stateExecutor);
            if (parent != nullptr) {
                return parent;
            }
        }
    }
    return nullptr;
}

void TaskMachineRunner::bindExecutorBaseInfo(QAbstractState* state, const TaskMachine::ConfigFlowExecutor *executor) {

    const auto callProperties = [this, executor] (bool entered) {
        for (const auto& prop : executor->properties()) {
            if (entered != prop.callOnEntered()) {
                continue;
            }
            QVariant data;
            if (prop.valueType() == "int") {
                data = prop.value().toInt();
            } else if (prop.valueType() == "bool") {
                data = prop.value().toLower() == "true";
            } else if (prop.valueType() == "string") {
                data = prop.value();
            } else if (prop.valueType() == "double") {
                data = prop.value().toDouble();
            } else if (prop.valueType() == "float") {
                data = prop.value().toFloat();
            } else if (prop.valueType() == "hex") {
                data = QByteArray::fromHex(prop.value().toLatin1());
            } else {
                printLog(QtMsgType::QtCriticalMsg, "unknown property type:" + prop.valueType());
            }
            if (data.isValid()) {
                currentBindContext->setProperty(prop.key().toLatin1(), data);
            }
        }
    };

    connect(state, &QAbstractState::entered, [=] {
        if (executor->printOnEnter()) {
            printLog(QtMsgType::QtInfoMsg, "state '" + executor->text() + "' enter!");
        }
        if (!executor->enter().isEmpty() || !executor->properties().isEmpty()) {
            callProperties(true);
            if (!executor->enter().isEmpty()) {
                invokeFunction(executor->enter());
            }
        }
#ifdef QTASK_MACHINE_REMOTE_DEBUG_ENABLED
        RemoteDebugListener::instance().activeNode(configFlow.name(), QDateTime::currentMSecsSinceEpoch(), executor->uuid());
#endif
    });

    connect(state, &QAbstractState::exited, [=] {
        if (executor->printOnExit()) {
            printLog(QtMsgType::QtInfoMsg, "state '" + executor->text() + "' exit!");
        }
        if (!executor->exit().isEmpty() || !executor->properties().isEmpty()) {
            callProperties(false);
            if (!executor->exit().isEmpty()) {
                invokeFunction(executor->exit());
            }
        }
    });
}

void TaskMachineRunner::invokeFunction(const QString &slotName) {
    if (slotName.isEmpty()) {
        return;
    }
    auto checkFunc = findFunction(slotName);
    bool invokeResult = checkFunc.invoke(currentBindContext, Qt::DirectConnection);
    if (!invokeResult) {
        printLog(QtMsgType::QtCriticalMsg, "cannot find slot function:" + slotName + " in context:" + currentBindContext->metaObject()->className());
        return;
    }
}

QMetaMethod TaskMachineRunner::findFunction(const QString &signalName) {
    if (signalName.isEmpty()) {
        return {};
    }
    auto functionName = signalName.toLatin1();
    int methodIndex = currentBindContext->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(functionName));
    if (methodIndex == -1) {
        printLog(QtMsgType::QtCriticalMsg, "cannot find function:" + signalName + " in context:" + currentBindContext->metaObject()->className());
        return {};
    }
    return currentBindContext->metaObject()->method(methodIndex);
}

void TaskMachineRunner::printLog(QtMsgType msgType, const QString &message) {
    if (debugPtr) {
        switch (msgType) {
            case QtCriticalMsg:
                qCCritical(debugPtr) << message;
                break;
            case QtDebugMsg:
                qCDebug(debugPtr) << message;
                break;
            case QtInfoMsg:
                qCInfo(debugPtr) << message;
                break;
            case QtWarningMsg:
                qCWarning(debugPtr) << message;
                break;
            default:
                break;
        }
    }
#ifdef QTASK_MACHINE_REMOTE_DEBUG_ENABLED
    RemoteDebugListener::instance().sendLog(configFlow.name(), QDateTime::currentMSecsSinceEpoch(), message, msgType);
#endif
}


bool BindableCheckEventState::testFinishBySignalData(const QVariantList &data)  {
    if (checkFunction.isValid()) {
        bool boolData = true;
        auto returnValue = Q_RETURN_ARG(bool, boolData);
        bool invokeResult = checkFunction.invoke(currentContext, Qt::DirectConnection, returnValue, Q_ARG(QVariantList, data));
        if (!invokeResult) {
            emit checkFunctionInvokeFailed(functionName);
            return true;
        }
        return boolData;
    }
    return true;
}

bool BindableMultiCheckEventState::testFinishBySignalData(int signalIndex, const QVariantList &data) {
    auto checkFunc = checkFunction[signalIndex];
    if (checkFunc.second.isValid()) {
        bool boolData = true;
        auto returnValue = Q_RETURN_ARG(bool, boolData);
        bool invokeResult = checkFunc.second.invoke(currentContext, Qt::DirectConnection, returnValue, Q_ARG(int, checkFunc.first), Q_ARG(QVariantList, data));
        if (!invokeResult) {
            emit checkFunctionInvokeFailed(functionNames[signalIndex]);
            return true;
        }
        return boolData;
    }
    return true;
}
