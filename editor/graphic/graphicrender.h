#pragma once

#include <qobject.h>
#include <qpainter.h>

#include "graphic/utils/transform.h"

#include "graphicobjecttype.h"

class GraphicRenderInterface {
public:
    virtual ~GraphicRenderInterface() = default;

    /**
     * @brief 重新生成对象绘制缓存
     */
    virtual void drawObject();

public:
    GraphicTransform graphicTransform;
    QPainter* renderPainter = nullptr;

protected:
    QSharedPointer<QPainter> cachePainter;

protected:
    /**
     * @brief 通过上中心点，获取节点主体背景框矩形，未进行Gui变换
     * @param topCenter
     * @param requiredWidth
     * @param requiredHeight
     * @return
     */
    static QRectF getNodeBodyRectFromTopCenter(const QPointF& topCenter, int requiredWidth, int requiredHeight);

    /**
     * @brief 获取文本宽度
     * @param text
     * @param pixelSize
     * @return
     */
    int getTextWidthByFont(const QString& text, int pixelSize) const;

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
    void drawNodeBody(const QRectF& rect);

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
     * @param padding
     */
    void drawNodeTitle(const QRectF& renderRect, const QString& title, int pixelSize, int padding);

    /**
     * @brief 绘制可连接的节点
     * @param renderRect
     * @param title
     * @param pixelSize
     * @param color
     * @param alignLeft
     */
    void drawConnectableItem(const QRectF& renderRect, const QString& title, int pixelSize, const QColor& color, bool alignLeft);

    /**
     * @brief 绘制双行可连接的节点
     * @param renderRect
     * @param title
     * @param subTitle
     * @param pixelSize
     * @param color
     * @param alignLeft
     */
    void drawDoubleRowConnectableItem(const QRectF& renderRect, const QString& title, const QString& subTitle, int pixelSize, const QColor& color, bool alignLeft);

    /**
     * @brief 绘制属性标题
     * @param renderRect
     * @param title
     * @param pixelSize
     * @param padding
     */
    void drawPropertyTitle(const QRectF& renderRect, const QString& title, int pixelSize, int padding);

    /**
     * @brief 绘制属性行
     * @param renderRect
     * @param title
     * @param pixelSize
     * @param color
     * @param alignLeft
     */
    void drawPropertyRow(const QRectF& renderRect, const QString& title, int pixelSize, const QColor& color, bool alignLeft = true);

    /**
     * @brief 绘制图标行
     * @param renderRect
     * @param iconPath
     * @param iconSize
     * @param displayText
     * @param pixelSize
     * @param alignLeft
     */
    void drawIconRow(const QRectF& renderRect, const QString& iconPath, int iconSize, const QString& displayText, int pixelSize, bool alignLeft);
};
