---
sidebar_position: 1
---

# 简介

这个类库是对`QState`的封装，与`QState`功能一致，主要功能为简化一些常用的状态转换。

## 支持的状态类型

|类|说明|使用场景|
|:--|:--|:--|
|DirectState|直接状态|用于执行流程中的一些操作，执行后立即切换|
|DelayState|延时状态|用于等待指定延时时间后再切换状态|
|EventState|事件触发状态|用于等待正常信号或异常信号之后再进行切换|
|MultiEventState|多事件状态|用于监听多个信号触发之后再进行切换|
|GroupState|状态组|可以作为其他状态的父状态，父状态需要等待子状态执行完成再切换|

例如下面的状态机示例：
```cpp
auto machine = new QStateMachine;

auto s1 = new QState(machine);
connect(s1, &QState::entered, this, [] {
    qDebug() << "s1 entered...";
});

auto s2 = new QState(machine);
connect(s2, &QState::entered, this, [] {
    qDebug() << "s2 entered...";
});

auto s3 = new QFinalState(machine);
connect(s3, &QFinalState::entered, this, [] {
    qDebug() << "s3 finial...";
});

s1->addTransition(s2);
s2->addTransition(ui.quitButton, &QPushButton::clicked, s3);

machine->setInitialState(s1);
machine->start();
```

使用`QStateMachine`的类编写：
```cpp
auto machine = new QStateMachine;

auto s1 = new DirectState(machine);
s1->bindState(TaskStateType::State_Enter, this, [] {
    qDebug() << "s1 entered...";
});

auto s2 = new EventState(machine);
s2->setSignal(ui.quitButton, &QPushButton::clicked);
s2->bindState(TaskStateType::State_Enter, this, [] {
    qDebug() << "s2 entered...";
});

auto s3 = new QFinalState(machine);
connect(s3, &QFinalState::entered, this, [] {
    qDebug() << "s3 finial...";
});

*s1 >> s2 >> s3;

machine->setInitialState(s1);
machine->start();
```

## 状态机编辑器

执行一些简单的流程时，使用基本的几个类型即可。但有时候随着业务逻辑变得复杂起来时，使用状态机编辑器定义状态切换会让状态机执行变得简单起来。

![Editor](/img/edit_demo.png)

状态机编辑器将状态保存为配置文件，执行一个状态机流程：
```cpp
#include <stateutils/taskmachinerunner.h>

auto runner = new TaskMachineRunner("state_flow1", this);
runner->run(context);
```

具体的使用方法看[编辑器说明文档](/editor/intro.md)。