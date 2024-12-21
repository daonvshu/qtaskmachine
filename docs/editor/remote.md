---
sidebar_position: 4
---

# 远程监控

在标题栏点击 ![add](https://raw.githubusercontent.com/daonvshu/qtaskmachine/refs/heads/main/editor/res/monitor.svg) 图标开启远程监控。

## 远程调试连接

编译核心库时，加入 [ProtocolCodecEngine](https://github.com/daonvshu/ProtocolCodecEngine) 帧协议编码库，配合编辑器使用远程调试功能。当连接到运行状态机的程序时，下面日志中实时打印当前执行的步骤，并且在流程图界面也会使用绿色背景标记当前正在执行的状态机：

![common_state](/img/remote_debug.png)

默认情况下，连接端口为5678，也可以在运行端加载配置时设置端口号：

```cpp
#include <QApplication>
#include <stateutils/taskmachinesteputil.h>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

    //...
    TaskMachineStepUtil::stepConfig(":/test1.json", 5678);
    //...

    return a.exec();
}
```

## 回溯

点击“回溯”按钮，打开回溯控制页面。回溯页面中，可以手动查看状态机依次执行的顺序，也可以根据设定间隔自动按照执行顺序运行一遍，同时流程图编辑页面也会标记当前执行的状态。

![common_state](/img/remote_backstrack.png)