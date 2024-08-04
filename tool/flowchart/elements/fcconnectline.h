#pragma once

#include <qobject.h>
#include <qgraphicsitem.h>

#include "utils/curvemovecontrol.h"

class FlowChartExecutorItem;
class FcConnectLine : public QGraphicsPathItem {
public:
    explicit FcConnectLine(QGraphicsItem *parent = nullptr);

    void refreshConnectPath(bool updateControlPoint = false);

public:
    FlowChartExecutorItem* connectFrom = nullptr;
    FlowChartExecutorItem* connectTo = nullptr;

    CurveMoveControl ctl;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    bool isMoving = false;
    QPointF offset;
    bool isHover = false;
};
