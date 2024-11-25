---
sidebar_position: 4
---

# 多事件触发状态

通过绑定多个信号，触发不同的状态转移。添加的信号数量必须和切换的目标状态数量一致。  

通过`addSignal`函数，依次绑定信号，绑定的顺序与状态切换的顺序一致：

```cpp
auto eventState = new MultiEventState(machine);
//设置监听信号
eventState->addSignal(trigger, &MultiEventTrigger::event1);
eventState->addSignal(trigger, &MultiEventTrigger::event2);
eventState->addSignal(trigger, &MultiEventTrigger::event3);
```

定义分支状态转移时，通过运算符`<<`添加切换目标：

```cpp
*eventState << event1State << event2State << event3State >> finishState;
```

使用示例如下：

```cpp
class MultiEventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void event1();
    void event2();
    void event3();
};

class MultiEventStateRun : public QObject {
public:
    static QStateMachine* run(MultiEventTrigger* trigger) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto eventState = new MultiEventState(machine);
        //设置监听信号
        eventState->addSignal(trigger, &MultiEventTrigger::event1);
        eventState->addSignal(trigger, &MultiEventTrigger::event2);
        eventState->addSignal(trigger, &MultiEventTrigger::event3);
        //进入状态监听
        eventState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "event state entered...";
        });
        //退出状态监听
        eventState->bindState(TaskStateType::State_Exit, machine, [&] {
            qDebug() << "event state exited...";
        });

        auto event1State = new QState(machine);
        connect(event1State, &QState::entered, machine, [&] {
            qDebug() << "event1 state entered...";
        });

        auto event2State = new QState(machine);
        connect(event2State, &QState::entered, machine, [&] {
            qDebug() << "event2 state entered...";
        });

        auto event3State = new QState(machine);
        connect(event3State, &QState::entered, machine, [&] {
            qDebug() << "event3 state entered...";
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> eventState << event1State << event2State << event3State >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};
```

触发event2时的打印：

```log
[20:31:33.889] -> begin state run...
[20:31:33.889] -> event state entered...
[20:31:34.674] -> event state exited...
[20:31:34.674] -> event2 state entered...
[20:31:34.674] -> final...
```