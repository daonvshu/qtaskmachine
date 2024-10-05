#pragma once

#include <qobject.h>
#include <qpainter.h>

#include "../utils/transform.h"

#include "../graphicobjecttype.h"

class GraphicRenderInterface {
public:
    void drawGraphicObject(QPainter* painter, bool isActiveState);

    virtual GraphicRenderInterface* getRender();

    virtual ~GraphicRenderInterface() = default;

public:
    GraphicTransform graphicTransform;

protected:
    virtual void drawObject(bool isActiveState);

    /**
     * @brief 绘制节点主体背景框
     * @param topCenter
     * @param requiredWidth
     * @param subWindowHeight
     * @param isSelected
     * @return
     */
    QRectF drawNodeBody(const QPointF& topCenter, int requiredWidth, int subWindowHeight, bool isSelected);

    /**
     * @brief 绘制节点分割线
     * @param nodeBodyRect
     * @param objectType
     */
    void drawNodeSplitLine(const QRectF& nodeBodyRect, GraphicObjectType objectType);

    /**
     * @brief 为对象绘制阴影
     * @param shadowObjectPixCache 对象临时缓冲图形
     * @param pos 绘制位置
     * @param blurRadius 阴影半径
     * @param color 阴影颜色
     * @param offset 偏移
     */
    void drawShadow(const QPixmap& shadowObjectPixCache, const QPointF& pos, qreal blurRadius, const QColor& color, const QPointF& offset);

protected:
    QPainter* renderPainter = nullptr;
};
