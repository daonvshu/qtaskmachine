---
sidebar_position: 1
---

# 简介

状态机编辑器用于解决大型复杂流程的执行，分离状态机的定义和实现。编辑器专注于配置流程，而编程端专注于在每个步骤具体做的事情。

## 基本原理和使用

状态机编辑器使用的是 **json** 配置文件，例如下面的流程图：

![Editor](/img/edit_demo.png)

将生成的 **json** 配置文件放在项目的 **qrc** 资源文件中，使用`TaskMachineStepUtil`类加载并解析配置文件：

```cpp
#include <QApplication>
#include <stateutils/taskmachinesteputil.h>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

    //...
    TaskMachineStepUtil::stepConfig(":/test1.json");
    //...

    return a.exec();
}
```

:::note

配置文件在任何流程执行之前，只需要加载一次即可，多次加载反而可能会出错。

:::

在任何需要执行流程图的地方，使用`TaskMachineRunner`类执行：

```cpp
auto runner = new TaskMachineRunner("test_flow1", parent);
if (!runner->run(context)) {
    qCritical() << "run task test_flow1 fail!";
    runner->deleteLater();
}
```

其中，`TaskMachineRunner`继承至`QObject`，因此`parent`仅为传递给`QObject`用于自动内存管理。`context`（执行类）用于具体定义所有步骤需要做的事情，后续文章中会逐一说明每个状态如何对应于执行类中的函数。

## 执行类的函数定义

编辑器配置中的每个步骤定义了调用执行类中的哪些函数，`TaskMachineRunner`调用执行类中的函数，其基本原理是使用Qt中的信号槽和反射，因此执行类通常会定义以下几种函数提供给`TaskMachineRunner`调用：

1. 属性

状态进入或者退出时可以设置执行类的属性值，条件状态的判断也可以根据属性值进行判断：

```cpp
class MyClass : public QObject {
    Q_OBJECT

    Q_PROPERTY(int prop1 READ getProp1 WRITE setProp1)
    Q_PROPERTY(int prop2 MEMBER prop2)

public:
    int getProp1() const;
    void setProp1(int value);

private:
    int prop1;
    int prop2;
}
```

2. 信号和槽函数

状态进入或者退出时可以调用指定槽函数，事件触发类型状态会使用信号触发状态转移：

```cpp
class MyClass : public QObject {
    Q_OBJECT

public:
    //...

signals:
    void event1Trigger();

private slots:
    void onState1Entered();
}
```

3. 可反射函数

条件判断类型状态会通过反射调用函数：

```cpp
class MyClass : public QObject {
    Q_OBJECT

public:
    //...

private:
    Q_INVOKABLE int testCondition1();
}
```