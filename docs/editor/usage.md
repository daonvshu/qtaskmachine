---
sidebar_position: 2
---

# 基本使用

## 配置文件和流程

在编辑流程图前，首先创建一个新的json配置文件，或者打开现有的配置。点击标题栏 **新建** 创建新的配置文件，点击标题栏![add](https://raw.githubusercontent.com/daonvshu/qtaskmachine/refs/heads/main/editor/res/add.svg)图标向组里面添加一个新的流程，一个配置文件中有多个流程图，每个流程的编辑和运行都是独立的。

通过标题栏上的按钮，可以添加、删除、编辑和复制一个流程图。

![title_config](/img/title_config.png)

构造`TaskMachineRunner`时，传入的名称必须和该处流程图的名称一致，为了避免编码造成无法识别，流程图的名称尽量使用英文命名。

```cpp
auto runner = new TaskMachineRunner("test_flow1", parent);
runner->run(context);
```

## 启用日志

默认情况下，TaskMachineRunner只会打印 `Warning` 和 `Critical` 等级的日志，使用 `setLogLevels` 函数调整日志等级：

```cpp
auto runner = new TaskMachineRunner("test_flow1", parent);
runner->setLogLevels(TaskMachineRunner::Debug | TaskMachineRunner::Info);
runner->run(context);
```

也可以使用日志筛选器进行控制：

```cpp
Q_LOGGING_CATEGORY(my_state, "test.my_state")

auto runner = new TaskMachineRunner("test_flow1", parent);
runner->setLogging(my_state, TaskMachineRunner::All);
runner->run(context);

```

## 流程图的绘制

在空白处点击右键，选择并添加不同类型的状态，不同的状态有不同数量的输入和输出端口，连接状态的输出到下一个状态的输入则表示一个状态切换关系。通常情况下，输入端口可以被多个状态连接，而输出端口只能连接到一个输入端口。

![menu](/img/add_menu.png)

各种状态作用和核心库中的类大致上是一致的，下面是每个状态的简要说明：

|状态|说明|
|:--|:--|
|开始状态|开始状态必须添加，且只能添加一个，流程的运行从该处开始。|
|结束状态|用于流程的结束，或状态组的子流程结束。不是必要的，没有该状态表示不会结束。|
|普通状态|用于流程中其中一个步骤的执行，不会等待，立即切换到下一个状态。|
|延时状态|设定指定延时时间后切换到下一个状态。|
|事件触发状态|等待指定信号的到来再切换到下一个状态，分为正常和异常分支，可以等待不同的信号进行切换。|
|复合事件状态|可以设置多个分支，每个分支等待各自的信号进行状态切换。|
|条件分支状态|可以设置多个分支，通过判断函数的结果进行判断再切换分支。|
|子状态组|状态组的开始，该状态必须等待子状态执行完成（切换到结束状态）或触发了异常信号才能进行状态切换。|
|恢复点|状态组异常信号触发时，切换到该状态可以恢复到子状态最后一次执行的状态。|
|循环执行状态|按照设定次数循环执行一组子状态|

## 状态的配置

选中一个状态，右键点击**编辑**，进入到状态属性编辑页面，属性页里面的设置用来和执行类进行交互，可以设置属性或调用函数，后续文章中详细描述每个状态的属性作用。