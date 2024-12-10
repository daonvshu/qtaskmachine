#pragma once

#include <qobject.h>
#include <qpixmap.h>

struct GraphicObjectData {
    QPointF renderPosition; // 绘图位置
    QPointF oldRenderPosition; // 旧的绘图位置
    bool selected = false;  // 是否选中
    bool isChanged = true;  // 是否有修改
    bool isRunning = false; // 是否正在运行该状态

    bool assignRemoved = false; // 是否标记为被删除

    QPixmap objectRenderCache; // 缓存

    void saveRenderPosition() {
        oldRenderPosition = renderPosition;
    }

    bool renderPositionChanged() const {
        return renderPosition != oldRenderPosition;
    }
};