---
sidebar_position: 1
---

# 直接状态

直接顺序执行的状态，不会等待任何事件，entered与exited事件会立即顺序触发。  

使用示例如下：

```cpp
auto machine = new QStateMachine;
//状态定义
auto beginState = new DirectState(machine);
beginState->bindState(TaskStateType::State_Enter, machine, [&] {
    qDebug() << "begin state run...";
});

auto secondState = new DirectState(machine);
secondState->bindState(TaskStateType::State_Enter, machine, [&] {
    qDebug() << "second state run...";
});

auto finalState = new QFinalState(machine);
connect(finalState, &QState::entered, machine, [&] {
    qDebug() << "final...";
});

//状态切换
*beginState >> secondState >> finalState;

//初始化并开始
machine->setInitialState(beginState);
machine->start();
```

执行上面的代码，将依次打印：

```log
[19:26:05.279] -> begin state run...
[19:26:05.280] -> second state run...
[19:26:05.280] -> final...
```