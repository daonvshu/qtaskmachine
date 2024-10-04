#pragma once

#include <qobject.h>
#include <qtransform.h>

#include "../graphiccontrol.h"

class TransformControl : public GraphicControl {
public:
    explicit TransformControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent = nullptr);

    /**
     * @brief 进行默认缩放，第一次绘制时进行放大
     */
    void scaleByDefault();

    /**
     * @brief 重置坐标轴变换
     */
    void coordinateReload();

    /**
     * @brief 移动变换开始
     * @param mousePoint
     */
    void moveBegin(const QPoint& mousePoint);

    /**
     * @brief 鼠标拖动
     * @param mousePoint
     */
    void dragMoving(const QPoint& mousePoint);

    /**
     * @brief 缩放变换
     * @param zoomIn
     * @param mousePoint
     * @return
     */
    bool scale(bool zoomIn, const QPointF& mousePoint);

    /**
     * @brief 应用其他变换
     * @param transform
     */
    void applyTransform(const QTransform& transform);

private:
    QPointF lastMoveBeginPos;
    QPointF oldViewCenter;

    bool isFirstLoad = true;
};
