---
sidebar_position: 2
---

# 安装

## 安装环境

- 测试使用的是`Qt5.15.2`版本，其他版本可能需要你自己测试并适配。
- 支持`c++14`标准的编译器

## 编译安装核心类库

进入到`src`目录，并创建`build`文件夹，进入到`build`文件夹中。

1. 如果使用`msvc`，可以使用如下命令：

```bash
# 执行cmake配置
cmake .. -DCMAKE_PREFIX_PATH=path/to/your/qt/library -DCMAKE_INSTALL_PREFIX=path/to/your/installdir/qtaskmachine -G "Visual Studio 17 2022"
# 执行Debug编译和安装
cmake --build . --config Debug --target INSTALL
```

2. 如果使用`gcc`，可以使用如下命令：

```bash
# 配置为Debug
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=path/to/your/qt/library -DCMAKE_INSTALL_PREFIX=path/to/your/installdir/qtaskmachine
# 执行编译和安装
make && make install
```

## 导入到工程中

如果是直接，可以到安装目录中直接找到头文件和库进行链接。如果使用`cmake`导入库，可以在`CMakeLists.txt`脚本中添加如下代码：

```bash
list(APPEND CMAKE_PREFIX_PATH path/to/your/installdir)

find_package(QTaskMachines REQUIRED)

target_link_libraries(${PROJECT_NAME} qtask_machines)
```
