---
sidebar_position: 6
---

# 复合事件状态

除了基础属性设置，还可以设置：触发信号列表。设置多个触发信号，可切换到不同分支。

![common_state](/img/props/multi_event_state.png)

## 分支ID

用于区分不同分支时使用，需要保证唯一。

## 信号函数

执行类定义的信号函数，等待该信号函数触发状态切换。

## 信号检查函数（可选）

信号函数可以携带参数，信号检查函数是用于检查参数是否是预期的值，返回false表示忽略该信号，使用以下函数签名的可反射函数：

```cpp
Q_INVOKABLE bool function(const QVariantList& data)
```