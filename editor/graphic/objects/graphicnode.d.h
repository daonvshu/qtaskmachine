#pragma once

#include "../graphicobject.d.h"

#include <qrect.h>
#include <qpixmap.h>

struct GraphicNodeData : GraphicObjectData {
    QString nodeName;   //节点名称

    QString funcEnter;  //进入状态回调函数
    QString funcExit;   //退出状态回调函数

    QRectF boundingRect; //节点边界
    QPixmap nodeBackgroundCache; //节点背景缓存，当boundingRect改变时，重新计算缓存
};