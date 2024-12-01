---
sidebar_position: 2
---

# 条件状态切换

所有基础状态可以设置多个切换目标并按照条件切换，一般情况下调用`>>`表示切换到下一个目标状态，而选择性切换到多个目标使用`<<`添加。

## 设置切换分支

使用`<<`选择可以切换的下一个分支：

```cpp
auto s1 = new DirectState;
auto s2 = new DirectState;
auto s3 = new DirectState;
auto selectState = new DirectState;
auto nextState = new DirectState;

*selectState << s1 << s2 << s3 >> nextState;
```

运算符`<<`将下个分支添加到该分支，并且返回该分支引用，当下一次使用`>>`时，隐式包含从分支状态切换到下一个分支。例如，如果选择切换到**s1**，则包含下面的状态转移关系：

```cpp
selectState >> s1 >> nextState;
```

要实现`A->B->C`或`A->C`这样的三角切换，需要将下个状态也添加到分支状态中：

```cpp
*selectState << nextState << s1 >> nextState;
```

## 设置分支条件

使用`setCondition`函数选择切换的分支索引，在状态进入**enter**之前设置生效：

```cpp
auto s1 = new DirectState;
auto s2 = new DirectState;
auto s3 = new DirectState;
auto selectState = new DirectState;

auto beginState = new DirectState;
beginState->bindState(TaskStateType::State_Enter, this, [=] {
    selectState->setCondition(0);
});

auto nextState = new DirectState;

*beginState >> selectState << s1 << s2 << s3 >> nextState;
```

`setCondition`函数也可以传递一个lambda表达式，在自身进入**enter**时调用：

```cpp
selectState->setCondition([] {
    return 0;
});
```

调用`setConditionDeferrable`函数，让lambda表达式在进入**exit**时延迟调用进行选择：

```cpp
selectState->setConditionDeferrable();
```