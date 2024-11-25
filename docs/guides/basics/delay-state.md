---
sidebar_position: 2
---

# 延时状态

延时执行的状态，用于延时指定时间后切换到下一个状态，可以监听entered和exited信号用来执行需要等待指定时间的两个步骤。  

在定义状态机流程时，通过构造函数传入等待时间(ms)：
```cpp
auto delayState = new DelayState(2000, parentState);
```

使用示例如下：

```cpp
auto machine = new QStateMachine;

//状态定义
auto beginState = new DirectState(machine);
beginState->bindState(TaskStateType::State_Enter, machine, [&] {
    qDebug() << "begin state run...";
});

auto delayState = new DelayState(2000, machine);
delayState->bindState(TaskStateType::State_Enter, machine, [&] {
    qDebug() << "delay before...";
});
delayState->bindState(TaskStateType::State_Exit, machine, [&] {
    qDebug() << "delay after...";
});

auto finalState = new QFinalState(machine);
connect(finalState, &QState::entered, machine, [&] {
    qDebug() << "final...";
});

//状态切换
*beginState >> delayState >> finalState;

//初始化并开始
machine->setInitialState(beginState);
machine->start();
```

执行上面的代码，将依次打印：

```log
[19:42:59.437] -> begin state run...
[19:42:59.437] -> delay before...
[19:43:01.444] -> delay after...
[19:43:01.445] -> final...
```