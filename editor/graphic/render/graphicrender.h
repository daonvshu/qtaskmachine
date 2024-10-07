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
     * @brief 通过上中心点，获取节点主体背景框矩形，未进行Gui变换
     * @param topCenter
     * @param requiredWidth
     * @param subWindowHeight
     * @return
     */
    static QRectF getNodeBodyRectFromTopCenter(const QPointF& topCenter, int requiredWidth, int subWindowHeight);

    /**
     * @brief 获取文本宽度
     * @param text
     * @param pixelSize
     * @return
     */
    int getTextWidthByFont(const QString& text, int pixelSize);

    /**
     * @brief 根据连接节点获取连接点位置
     * @param itemRect
     * @param onLeft
     * @return
     */
    static QRectF getConnectPointRect(const QRectF& itemRect, bool onLeft);

    /**
     * @brief 绘制节点主体背景框
     * @param rect
     * @param nodeData
     */
    void drawNodeBody(const QRectF& rect, QSharedPointer<struct GraphicNodeData>& nodeData);

    /**
     * @brief 绘制节点分割线
     * @param nodeBodyRect
     * @param objectType
     */
    void drawNodeSplitLine(const QRectF& nodeBodyRect, GraphicObjectType objectType);

    /**
     * @brief 为对象绘制阴影
     * @param shadowObjectPixCache 对象临时缓冲图形
     * @param blurRadius 阴影半径
     * @param color 阴影颜色
     * @param offset 偏移
     * @return 返回阴影背景
     */
    static QImage drawShadow(const QPixmap& shadowObjectPixCache, qreal blurRadius, const QColor& color, const QPointF& offset);

    /**
     * @brief 绘制节点标题
     * @param renderRect
     * @param title
     * @param pixelSize
     */
    void drawNodeTitle(const QRectF& renderRect, const QString& title, int pixelSize);

    /**
     * @brief 绘制可连接的节点
     * @param renderRect
     * @param title
     * @param pixelSize
     * @param color
     * @param onLeft
     */
    void drawConnectableItem(const QRectF& renderRect, const QString& title, int pixelSize, const QColor& color, bool onLeft);

protected:
    QPainter* renderPainter = nullptr;
};
