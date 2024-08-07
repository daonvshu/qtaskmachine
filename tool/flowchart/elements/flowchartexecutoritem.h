#pragma once

#include <qobject.h>
#include <qgraphicsitem.h>

class FcConnectLine;
class FlowChartExecutorItem : public QGraphicsRectItem {
public:
    explicit FlowChartExecutorItem(QGraphicsItem *parent = nullptr);

    void setPos(const QPointF& pos, QGraphicsScene *scene);

    void setTopLeftPos(const QPointF& pos, QGraphicsScene *scene);

    void updateGeometry(QGraphicsScene *scene);

    QPointF testInConnectPoint(const QPointF& mousePoint);

    int getConnectPointIndex(const QPointF& connectPoint);

    void setIsHoverState(bool isHover);

    void setConnectFromLine(FcConnectLine* connectLine);

    void setConnectToLine(FcConnectLine* connectLine);

    void disconnectLine(FcConnectLine* line);

    void disconnectLines();

    const QList<QPointF>& getConnectPoints() const {
        return connectPoints;
    }

    int getConnectedLineSize() const;

    virtual bool acceptableConnectLine();

    virtual bool creatableConnectLine();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    virtual QSizeF itemSize() const = 0;

protected:
    bool mouseHover = false;
    bool isDragging = false;
    QPointF offset;

    QList<QPointF> connectPoints;

    QHash<int, QList<FcConnectLine*>> connectTo;  //连接到下一个节点
    QHash<int, QList<FcConnectLine*>> connectFrom; //连接至上一个节点

private:
    void collidingDetect(QGraphicsScene *scene);
    void reCalcConnectPointPos();
    void updateConnectLines();
    void beginConnectLineMove();
};
