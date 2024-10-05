#pragma once

#include <qobject.h>

struct GraphicObjectData {
    QPointF renderPosition; // 绘图位置
    bool selected = false;  // 是否选中
};