---
sidebar_position: 5
---

# 配置文件

通过解析编辑器生成的配置文件，可以通过解析数据创建自己的状态机执行类。

基本结构：

```json
{
    "flows": [
        {
            "executors": [
                {
                    "condition": "",
                    "delay": 0,
                    "enter": "",
                    "exit": "",
                    "funcRetry": "",
                    "id": 1,
                    "nested": false,
                    "properties": [
                    ],
                    "retry": 0,
                    "taskId": 0,
                    "text": "开始",
                    "timeout": 0,
                    "type": 0,
                    "uuid": "{df7ac883-80cf-4d73-b54a-ecb2ec6bde61}",
                    "x": -465,
                    "y": -195
                }
            ],
            "lines": [
                {
                    "branchId": 0,
                    "branchName": "",
                    "checkFunc": "",
                    "connectFrom": 1,
                    "connectFromPIndex": 0,
                    "connectTo": 3,
                    "connectToPIndex": 0,
                    "ctlPx": 0,
                    "ctlPy": 0,
                    "failBranch": false,
                    "subBranch": false,
                    "trigger": ""
                }
            ],
            "name": "test_flow1",
            "version": 2
        }
    ]
}
```

## flows

flows节点为一个数组，一个配置文件可以包含多个流程图。一个流程图对象包含下面几个属性：

|字段|说明|
|:--|:--|
|executors|状态的部分属性配置，所有类型状态共用该结构。|
|lines|链接线属性，包含链接的开始状态和结束状态的端点信息。|
|name|流程图的名称。|
|version|流程图的版本号。|

## executors

executors包含所有状态的部分属性，每个状态通过id或uuid作为唯一标志区分状态。每个executor的属性说明：

|字段|类型|说明|
|:--|:--|:--|
|id|int|状态的唯一标志。每次绘图保存后，id可能会改变。|
|uuid|string|状态的唯一标志。绘图改变不会改变该uuid。|
|text|string|状态的名称。|
|taskId|int64|执行的任务id。v2版本已弃用。|
|x|double|绘图坐标系中的位置x。|
|y|double|绘图坐标系中的位置y。|
|type|int|状态的类型。参照下面状态类型表说明。|
|enter|string|状态进入时调用的槽函数。|
|exit|string|状态退出时调用的槽函数。|
|delay|int|延时状态使用，延时毫秒数。|
|timeout|int|事件触发状态使用，超时毫秒数。|
|retry|int|事件触发状态使用，超时重试次数。|
|funRetry|string|事件触发状态使用，超时触发时回调函数。|
|nested|bool|恢复点状态使用，是否是嵌套恢复点。|
|condition|string|条件状态使用，条件检查函数。|
|properties|list|参照下面property类型说明。|

状态类型表：

|值|状态类型|
|:--|:--|
|0|开始状态|
|1|结束状态|
|2|普通状态|
|3|延时状态|
|4|事件触发状态|
|5|复合事件状态|
|6|条件分支状态|
|7|子状态组|
|8|恢复点|

## property

executor中的property属性。

|字段|类型|说明|
|:--|:--|:--|
|callOnEntered|bool|进入还是退出状态时设置属性。|
|key|string|属性名称。|
|value|string|属性值。|
|valueType|string|属性类型，参考基础类型属性说明。|

## lines

lines包含所有状态的链接关系，包含部分状态的输入输出端点的属性。

|字段|类型|说明|
|:--|:--|:--|
|connectFrom|int|连接开始的状态id。|
|connectFromPIndex|int|连接开始状态端点index。对于条件分支、复合事件状态等，具有多个动态输出点列表的index。|
|connectTo|int|连接结束的状态id。|
|connectToPIndex|int|连接结束状态端点index。一般状态只有一个输入点，该值始终为0。|
|ctlPx|double|控制点坐标x，v2已弃用。|
|ctlPy|double|控制点坐标y，v2已弃用。|
|trigger|string|触发连接开始状态的分支端点使用的信号函数。|
|checkFunc|string|触发信号函数的检查函数。|
|branchId|int|分支id。|
|branchName|string|分支名称。|
|failBranch|bool|是否是异常分支。事件触发状态、状态组使用。|
|subBranch|bool|是否是子流程分支。状态组使用。|
