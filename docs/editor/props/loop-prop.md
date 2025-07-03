---
sidebar_position: 10
---

# 循环执行状态

循环执行状态是一个状态组，按照设置的次数循环调用子状态分支。

![common_state](/img/props/loop_state.png)

## 执行总次数

执行总次数表示子状态组执行的次数，子状态组需要执行到“结束状态”才能进行下一个循环。

## 退出检查函数

每次循环前会先调用退出检查函数判断是否提前退出循环。函数的声明是`Q_INVOKABLE bool function(int)`，传入的参数是当前循环的index，返回true表示结束当前循环。