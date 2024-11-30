---
sidebar_position: 1
---

# 状态分组

## 父状态

设置父状态为同一个状态将一系列状态进行分组，父状态需要等待子状态结束（切换到`QFinalState`），再切换到下一个状态：

```cpp
auto machine = new QStateMachine;

//状态定义
auto beginState = new DirectState(machine);
beginState->bindState(TaskStateType::State_Enter, machine, [&] {
    qDebug() << "begin state run...";
});

auto groupState = new GroupState(machine);
groupState->bindState(TaskStateType::State_Enter, machine, [&] {
    qDebug() << "group state entered...";
});
groupState->bindState(TaskStateType::State_Exit, machine, [&] {
    qDebug() << "group state exited...";
});

{
    auto s1 = new DirectState(groupState);
    s1->bindState(TaskStateType::State_Enter, machine, [&] {
        qDebug() << "group s1 enter...";
    });

    auto s2 = new DirectState(groupState);
    s2->bindState(TaskStateType::State_Enter, machine, [&] {
        qDebug() << "group s2 enter...";
    });

    auto finalState = new QFinalState(groupState);
    connect(finalState, &QState::entered, machine, [&] {
        qDebug() << "group final enter...";
    });

    //状态切换
    *s1 >> s2 >> finalState;

    groupState->setInitialState(s1);
}

auto finalState = new QFinalState(machine);
connect(finalState, &QState::entered, machine, [&] {
    qDebug() << "final...";
});

//状态切换
*beginState >> groupState >> finalState;

//初始化并开始
machine->setInitialState(beginState);
machine->start();
```

执行上面的代码依次打印：

```log
[18:51:46.079] -> begin state run...
[18:51:46.079] -> group state entered...
[18:51:46.080] -> group s1 enter...
[18:51:46.080] -> group s2 enter...
[18:51:46.080] -> group final enter...
[18:51:46.081] -> group state exited...
[18:51:46.081] -> final...
```

## 状态转移继承

子状态隐式继承所有父状态的状态切换，因此，当使用`EventState`作为父状态时，所有子状态也会响应正常和异常信号并切换到下一个分支或异常分支：

```cpp
class GroupEventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void finish();
    void fail();
};

class GroupStateRun : public QObject {
public:
    static QStateMachine* run(GroupEventTrigger* trigger) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto failState = new DirectState(machine);
        failState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "fail state run...";
        });

        auto groupState = new EventState(machine);
        //触发finish直接结束子状态，并切换到组状态下一个状态
        groupState->setSignal(trigger, &GroupEventTrigger::finish);
        //主动触发失败，从子状态直接切换到失败状态
        connect(trigger, &GroupEventTrigger::fail, groupState, &EventState::triggerSignalFail);
        groupState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "group state entered...";
        });
        groupState->bindState(TaskStateType::State_Exit, machine, [&] {
            qDebug() << "group state exited...";
        });

        {
            auto s1 = new DirectState(groupState);
            s1->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "group s1 enter...";
            });

            auto s2 = new DelayState(5000, groupState);
            s2->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "group s2 enter...";
            });
            s2->bindState(TaskStateType::State_Exit, machine, [&] {
                qDebug() << "group s2 exited...";
            });

            auto finalState = new QFinalState(groupState);
            connect(finalState, &QState::entered, machine, [&] {
                qDebug() << "group final enter...";
            });

            //状态切换
            *s1 >> s2 >> finalState;

            groupState->setInitialState(s1);
        }

        auto nextState = new DirectState(machine);
        nextState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "next state enter...";
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        groupState->setFailState(failState);
        *beginState >> groupState >> nextState >> finalState;
        *failState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};
```

上面的示例中，如果当前状态在`s2`延时状态，不做任何事将等待5s切换到`nextState`：

```log
[18:58:15.302] -> begin state run...
[18:58:15.303] -> group state entered...
[18:58:15.303] -> group s1 enter...
[18:58:15.303] -> group s2 enter...
[18:58:20.291] -> group s2 exited...
[18:58:20.292] -> group final enter...
[18:58:20.293] -> group state exited...
[18:58:20.293] -> next state enter...
[18:58:20.294] -> final...
```

而如果主动触发正常或异常分支，`s2`将立即退出：

```log
[19:00:55.495] -> begin state run...
[19:00:55.495] -> group state entered...
[19:00:55.496] -> group s1 enter...
[19:00:55.496] -> group s2 enter...
[19:00:56.645] -> group s2 exited...
[19:00:56.646] -> group state exited...
[19:00:56.646] -> next state enter...
[19:00:56.646] -> final...
```