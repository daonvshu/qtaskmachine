#pragma once

#include "../graphicobject.d.h"

#include "data/basepropertydata.h"

#include <qrect.h>
#include <qpixmap.h>

struct GraphicNodeData : GraphicObjectData {
    BasePropertyData propData; //节点数据

    QRectF boundingRect; //节点边界

    QList<QRectF> inputLinkPoints;  //输入链接点
    QList<QRectF> outputLinkPoints; //输出链接点
    int activeInputLinkPointIndex = -1;  //激活的输入链接点
    int activeOutputLinkPointIndex = -1; //激活的输出链接点

    QList<QColor> inputLinkPointColors; //输入链接点颜色
    QList<QColor> outputLinkPointColors; //输出链接点颜色
};