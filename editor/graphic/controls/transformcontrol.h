﻿#pragma once

#include <qobject.h>
#include <qtransform.h>

#include "../graphiccontrol.h"

class TransformControl : public GraphicControl {
public:
    explicit TransformControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent = nullptr);

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
     * @brief 将指定点移动到视图中心
     * @param targetRealPoint
     */
    void moveToCenter(const QPointF& targetRealPoint) const;

    /**
     * @brief 缩放变换
     * @param zoomIn
     * @param mousePoint
     * @return
     */
    bool scale(bool zoomIn, const QPointF& mousePoint);

    /**
     * @brief 重置变换参数
     * @param offsetX
     * @param offsetY
     */
    void resetTransform(qreal offsetX, qreal offsetY) const;

private:
    QPointF lastMoveBeginPos;
    QPointF oldViewCenter;

    bool isFirstLoad = true;
};
