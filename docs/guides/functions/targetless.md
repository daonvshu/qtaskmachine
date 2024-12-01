---
sidebar_position: 4
---

# 无目标状态切换

基础类型中`DelayState`、`EventState`、`GroupState`支持不设置切换目标，当没有切换目标时，状态退出后会立即重新进入自身。  

一个典型的例子，子状态中反复执行一个操作，知道父状态触发失败分支退出：

```cpp
auto machine = new QStateMachine;

//状态定义
auto beginState = new DirectState(machine);
auto failState = new DirectState(machine);

auto groupState = new EventState(machine);
connect(trigger, &GroupEventTrigger::fail, groupState, &EventState::triggerSignalFail);
{
    auto s1 = new DirectState(groupState);
    auto s2 = new EventState(groupState);
    s2->setSignal(trigger, &EventTrigger::finish);

    *s1 >> s2;
    groupState->setInitialState(s1);
}

auto nextState = new DirectState(machine);
auto finalState = new QFinalState(machine);

//状态切换
groupState->setFailState(failState);
*beginState >> groupState >> nextState >> finalState;
*failState >> finalState;

//初始化并开始
machine->setInitialState(beginState);
machine->start();
```