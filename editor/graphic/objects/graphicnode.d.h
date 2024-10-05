#pragma once

#include "../graphicobject.d.h"

struct GraphicNodeData : GraphicObjectData {
    QString nodeName;   //节点名称

    QString funcEnter;  //进入状态回调函数
    QString funcExit;   //退出状态回调函数
};