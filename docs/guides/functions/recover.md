---
sidebar_position: 3
---

# 状态恢复

使用`QHistoryState`保存历史状态，历史状态是用于保存一个状态组，当一个子状态发生某种异常切换到一个单独状态时，从这个单独的状态再切换到历史状态将重新执行上一个子状态。历史状态可以设置是否保存所有的嵌套子状态，默认情况下，只会保存当前状态组的第一层流程。

```cpp
class RecoverEventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void trigger();
    void recoverMain();
    void recoverChild();
};

class RecoverStateRun : public QObject {
public:
    static QStateMachine* run(RecoverEventTrigger* trigger) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto failState = new QState(machine);
        connect(failState, &QState::entered, machine, [&] {
            qDebug() << "fail state run...";
        });

        auto mainGroupState = new EventState(machine);
        //主动触发失败，从子状态直接切换到失败状态
        connect(trigger, &RecoverEventTrigger::trigger, machine, [=] {
            mainGroupState->triggerSignalFail();
        });

        //主状态组历史状态
        auto mainGroupHistory = new QHistoryState(mainGroupState);

        //子状态组历史状态
        auto childGroupHistory = new QHistoryState(QHistoryState::DeepHistory, mainGroupState);

        mainGroupState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "main group state entered...";
        });
        mainGroupState->bindState(TaskStateType::State_Exit, machine, [&] {
            qDebug() << "main group state exited...";
        });

        {
            auto s1 = new DirectState(mainGroupState);
            s1->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "group s1 enter...";
            });
            //设置历史默认状态
            mainGroupHistory->setDefaultState(s1);
            childGroupHistory->setDefaultState(s1);

            auto s2 = new DelayState(5000, mainGroupState);
            s2->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "group s2 enter...";
            });
            s2->bindState(TaskStateType::State_Exit, machine, [&] {
                qDebug() << "group s2 exited...";
            });

            auto childGroup = new GroupState(mainGroupState);
            childGroup->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "child group enter...";
            });
            childGroup->bindState(TaskStateType::State_Exit, machine, [&] {
                qDebug() << "child group exited...";
            });

            {
                auto s11 = new DirectState(childGroup);
                s11->bindState(TaskStateType::State_Enter, machine, [&] {
                    qDebug() << "group s11 enter...";
                });

                auto s12 = new DelayState(5000, childGroup);
                s12->bindState(TaskStateType::State_Enter, machine, [&] {
                    qDebug() << "group s12 enter...";
                });
                s12->bindState(TaskStateType::State_Exit, machine, [&] {
                    qDebug() << "group s12 exited...";
                });

                auto finalState = new QFinalState(childGroup);
                connect(finalState, &QState::entered, machine, [&] {
                    qDebug() << "child group final enter...";
                });

                //状态切换
                *s11 >> s12 >> finalState;

                childGroup->setInitialState(s11);
            }

            auto finalState = new QFinalState(mainGroupState);
            connect(finalState, &QState::entered, machine, [&] {
                qDebug() << "main group final enter...";
            });

            //状态切换
            *s1 >> s2 >> childGroup >> finalState;

            mainGroupState->setInitialState(s1);
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
        mainGroupState->setFailState(failState);
        failState->addTransition(trigger, &RecoverEventTrigger::recoverMain, mainGroupHistory);
        failState->addTransition(trigger, &RecoverEventTrigger::recoverChild, childGroupHistory);

        *beginState >> mainGroupState >> nextState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};
```

上面的例子中，如果进入到`s12`状态时，触发`mainGroupState`的异常分支，将从`s12`立即退出到`finalState`。如果从主状态恢复，则重新从`s11`开始执行：

```log
[16:09:08.577] -> begin state run...
[16:09:08.577] -> main group state entered...
[16:09:08.577] -> group s1 enter...
[16:09:08.578] -> group s2 enter...
[16:09:13.572] -> group s2 exited...
[16:09:13.573] -> child group enter...
[16:09:13.574] -> group s11 enter...
[16:09:13.575] -> group s12 enter...
[16:09:14.252] -> group s12 exited...
[16:09:14.252] -> child group exited...
[16:09:14.253] -> main group state exited...
[16:09:14.253] -> fail state run...
[16:09:16.876] -> main group state entered...
[16:09:16.876] -> child group enter...
[16:09:16.877] -> group s11 enter...
[16:09:16.877] -> group s12 enter...
[16:09:21.871] -> group s12 exited...
[16:09:21.872] -> child group final enter...
[16:09:21.873] -> child group exited...
[16:09:21.873] -> main group final enter...
[16:09:21.874] -> main group state exited...
[16:09:21.874] -> next state enter...
[16:09:21.875] -> final...
```

而如果从子状态恢复，则重新从`s12`处开始执行：

```log
[16:10:35.253] -> begin state run...
[16:10:35.253] -> main group state entered...
[16:10:35.253] -> group s1 enter...
[16:10:35.254] -> group s2 enter...
[16:10:40.245] -> group s2 exited...
[16:10:40.247] -> child group enter...
[16:10:40.247] -> group s11 enter...
[16:10:40.248] -> group s12 enter...
[16:10:41.831] -> group s12 exited...
[16:10:41.832] -> child group exited...
[16:10:41.832] -> main group state exited...
[16:10:41.833] -> fail state run...
[16:10:43.748] -> main group state entered...
[16:10:43.748] -> child group enter...
[16:10:43.748] -> group s12 enter...
[16:10:48.744] -> group s12 exited...
[16:10:48.745] -> child group final enter...
[16:10:48.745] -> child group exited...
[16:10:48.745] -> main group final enter...
[16:10:48.746] -> main group state exited...
[16:10:48.746] -> next state enter...
[16:10:48.747] -> final...
```