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
};