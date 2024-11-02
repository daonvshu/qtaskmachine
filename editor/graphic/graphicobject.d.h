#pragma once

#include <qobject.h>
#include <qpixmap.h>

struct GraphicObjectData {
    QPointF renderPosition; // 绘图位置
    bool selected = false;  // 是否选中
    bool isChanged = true;  // 是否有修改

    QPixmap objectRenderCache; // 缓存
};