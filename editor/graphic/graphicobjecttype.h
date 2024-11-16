#pragma once

enum class GraphicObjectType {
    Node_Begin_State,           // 开始状态
    Node_End_State,             // 结束状态
    Node_Normal_State,          // 普通状态
    Node_Delay_State,           // 延时状态
    Node_Event_State,           // 事件触发状态
    Node_MultiEvent_State,      // 复合事件状态
    Node_Condition_State,       // 条件分支状态
    Node_State_Group,           // 子状态组
    Node_Recovery_State,        // 恢复点

    Link_Line,                  // 连接线

    Action_Node_Move,           // 节点移动
    Action_Object_Remove,       // 对象删除
};