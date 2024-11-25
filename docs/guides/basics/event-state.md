---
sidebar_position: 3
---

# 事件触发状态

通过绑定到指定对象的信号来触发状态转移。  

## 绑定信号

`EventState`包含两个分支的状态转换，一个正常流程和一个异常流程。正常流程通过`setSignal`函数绑定一个信号：

```cpp
auto eventState = new EventState(parentState);
eventState->setSignal(trigger, &EventTrigger::finish);
```

异常信号不需要额外绑定信号，但定义分支的切换使用`setFailState`函数表示异常分支的状态转换：

```cpp
//正常分支切换
*eventState >> nextState;
//异常分支切换
eventState->setFailState(failState);
```

## 超时与异常切换

异常分支的切换主要有两种方式：直接由外部调用其函数`triggerSignalFail`，或者设置超时后触发：

```cpp
//直接触发异常分支状态切换
eventState->triggerSignalFail();
//设置等待3000ms后触发异常分支状态切换
eventState->setTimeout(3000);
```

:::tip

异常状态切换和超时时间的设置不是必要的，没有设置时将无限等待绑定的信号触发状态转移。

:::

## 重试

通过设置重试次数，当超时触发异常时，将重新进入当前状态继续等待信号触发，直到重试计数达到设置值：

```cpp
eventState->setRetrySize(3);
```

监听重试发生时，可以通过绑定`stateRetry`信号：

```cpp
connect(eventState, &EventState::stateRetry, this, [&] {
    qDebug() << "event state retry...";
});
```

:::warning

1. 调用主动触发异常的函数`triggerSignalFail`，将不会执行重试过程。
2. 触发重试时，当前状态会退出并重新进入，这就意味着`enter`和`exit`信号会再次被调用。

:::

## 事件触发参数检查

绑定的信号，其参数是可检查的，继承至`EventState`并重载其函数`testFinishBySignalData`可以检查信号的参数是否是想要的值，接收到的数据将转化为 **data** 数据列表，通过返回 **false** 可以阻止当前信号不触发状态转移：

```cpp
class WaitForTargetValueState : public EventState {
public:
    explicit WaitForTargetValueState(int targetValue, QState* parent)
        : EventState(parent)
        , targetValue(targetValue)
    {}

protected:
    bool testFinishBySignalData(const QVariantList &data) override {
        int cur = data.first().toInt();
        if (cur != targetValue) {
            qWarning() << "target value not match, value:" << cur << ", ignore...";
            return false;
        }
        return true;
    }

private:
    int targetValue;
};
```

## 使用示例

```cpp
class EventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void finish();
    void fail();
};

class EventStateRun : public QObject {
public:
    static QStateMachine* run(EventTrigger* trigger) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto failState = new DirectState(machine);
        failState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "fail state enter...";
        });

        auto eventState = new EventState(machine);
        //设置监听信号
        eventState->setSignal(trigger, &EventTrigger::finish);
        //设置失败切换状态
        eventState->setFailState(failState);
        //设置超时
        eventState->setTimeout(3000);
        //设置超时重试
        eventState->setRetrySize(1);
        //进入状态监听
        eventState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "event state entered...";
        });
        //退出状态监听
        eventState->bindState(TaskStateType::State_Exit, machine, [&] {
            qDebug() << "event state exited...";
        });
        //监听超时重试
        connect(eventState, &EventState::stateRetry, machine, [&] {
            qDebug() << "event state retry...";
        });
        //主动触发失败
        connect(trigger, &EventTrigger::fail, machine, [=] {
            eventState->triggerSignalFail();
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> eventState >> finalState;
        *failState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};
```

执行上面的代码，等待3s，重试1次时的打印：

```log
[20:08:54.255] -> begin state run...
[20:08:54.255] -> event state entered...
[20:08:57.241] -> event state retry...
[20:08:57.242] -> event state exited...
[20:08:57.242] -> event state entered...
[20:09:00.241] -> event state exited...
[20:09:00.242] -> fail state enter...
[20:09:00.243] -> final...
```