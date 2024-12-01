---
sidebar_position: 5
---

# 调试打印

设置状态名称标志和`LoggingCategory`，在状态进入和离开时打印信息。

```cpp
Q_DECLARE_LOGGING_CATEGORY(debugState)

class DebugStateRun : public QObject {
public:
    static void run() {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->setStateName("beginState", debugState);

        auto secondState = new DirectState(machine);
        secondState->setStateName("secondState", debugState);

        auto finalState = new QFinalState(machine);

        //状态切换
        *beginState >> secondState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();
    }
};
```

使用该方法可以知道当前正在运行到哪个状态中，并且可以使用[`QLoggingCategory`](https://doc.qt.io/qt-5/qloggingcategory.html#configuring-categories)的规则是否开启状态机的打印。

```log
[16:12:29.577] -> state '"beginState"' enter!
[16:12:29.578] -> state '"beginState"' exit!
[16:12:29.578] -> state '"secondState"' enter!
[16:12:29.579] -> state '"secondState"' exit!
```