#pragma once

#include "../graphicobject.d.h"

#include "data/basepropertydata.h"

#include <qrect.h>
#include <qpixmap.h>

struct GraphicNodeData : GraphicObjectData {
    BasePropertyData propData; //节点数据

    QRectF boundingRect; //节点边界
    QSizeF oldBackgroundGuiSize; //上一次缓存的背景大小
    QPixmap nodeBackgroundCache; //节点背景缓存，当boundingRect改变时，重新计算缓存

    QList<QRectF> inputLinkPoints;  //输入链接点
    QList<QRectF> outputLinkPoints; //输出链接点
    int activeInputLinkPointIndex = -1;  //激活的输入链接点
    int activeOutputLinkPointIndex = -1; //激活的输出链接点
};