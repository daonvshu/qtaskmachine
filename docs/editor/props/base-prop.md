---
sidebar_position: 1
---

# 普通状态

普通状态是所有状态中最基础和最直接的状态，所有状态的基础属性和普通状态一致，包括一下几个基础的设置项：状态名称、状态进入时调用、状态退出时调用、属性值应用。

![common_state](/img/props/common_state.png)

## 状态名称

用于状态的命名，也是状态运行时的打印名称。

## 状态进入时调用

设置状态进入时调用的函数，为了可以通过字符串查找函数，这里必须是信号函数、槽函数或可反射的函数，例如下面的函数：

```cpp
class MyClass : public QObject {
    Q_OBJECT
    //...
private slots:
    void onState1Entered();
}

```

在这里填写 `onState1Entered()` 即可。

## 状态退出时调用

设置状态退出时调用的函数，与进入设置的方式一样，这里也必须是信号函数、槽函数或可反射函数。

:::warning

需要特别注意的是，在子流程中设置退出时调用函数，不管是该流程正常切换还是子状态组遇到异常进行切换，状态退出函数都会被调用。也就是说子流程中不建议使用退出时调用函数，更好的办法是在其后连接一个新的普通流程。

:::

## 属性值应用

点击 ![add](https://raw.githubusercontent.com/daonvshu/qtaskmachine/refs/heads/main/editor/res/add.svg) 图标，添加一个属性值的设置。

|输入项|说明|
|:--|:--|
|进入/退出|设置状态进入还是退出时应用该属性设置|
|key|属性的名称，必须使用`Q_PROPERTY`定义属性|
|value|属性值|
|属性值类型|选择支持的6种属性类型设置：int/bool/string(QString)/double/float/hex(QByteArray)|