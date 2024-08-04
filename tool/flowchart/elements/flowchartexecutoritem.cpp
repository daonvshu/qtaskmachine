#include "flowchartexecutoritem.h"

#include "fcconnectline.h"

#include <QGraphicsSceneMouseEvent>
#include <qapplication.h>
#include <qgraphicsscene.h>
#include <QGraphicsDropShadowEffect>
#include <qpainter.h>
#include <qstyleoption.h>
#include <qdebug.h>

FlowChartExecutorItem::FlowChartExecutorItem(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    setZValue(0);

    // 添加阴影效果
    auto shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(6); // 阴影模糊半径
    shadow->setColor(0x6B6F79); // 阴影颜色
    shadow->setOffset(0, 0); // 阴影偏移量
    setGraphicsEffect(shadow);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsScenePositionChanges | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}

void FlowChartExecutorItem::setPos(const QPointF &pos, QGraphicsScene *scene) {
    auto rectSize = itemSize();
    setRect(pos.x() - rectSize.width() / 2.0, pos.y() - rectSize.height() / 2.0, rectSize.width(), rectSize.height());
    collidingDetect(scene);
    reCalcConnectPointPos();
}

void FlowChartExecutorItem::setTopLeftPos(const QPointF &pos, QGraphicsScene *scene) {
    auto rectSize = itemSize();
    setRect(pos.x(), pos.y(), rectSize.width(), rectSize.height());
    collidingDetect(scene);
    reCalcConnectPointPos();
}

void FlowChartExecutorItem::updateGeometry(QGraphicsScene *scene) {
    auto oldRect = rect();
    setTopLeftPos(oldRect.topLeft(), scene);
    updateConnectLines();
}

void FlowChartExecutorItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (!testInConnectPoint(event->pos()).isNull()) {
            return;
        }
        offset = event->pos() - rect().topLeft();
        isDragging = true;
        setZValue(1);
        beginConnectLineMove();
    }
    QGraphicsRectItem::mousePressEvent(event);
}

void FlowChartExecutorItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isDragging) {
        auto curRect = rect();
        curRect.moveTopLeft(event->pos() - offset);
        setRect(curRect);
        reCalcConnectPointPos();
        updateConnectLines();
    }
}

void FlowChartExecutorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent*) {
    if (isDragging) {
        collidingDetect(scene());
    }
    isDragging = false;
    setZValue(0);

    reCalcConnectPointPos();
    update();
}

void FlowChartExecutorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    //绘制连接点
    if (mouseHover && !isDragging) {
        painter->setBrush(QBrush(0x6A5ACD));
        painter->setPen(Qt::NoPen);
        for (const auto& p : connectPoints) {
            painter->drawEllipse(p, 5, 5);
        }
    }

    if (option->state.testFlag(QStyle::State_Selected)) {
        painter->save();
        auto pen = painter->pen();
        pen.setColor(0x026EC1);
        pen.setDashPattern({3, 2, 3, 2});
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        painter->drawRect(option->rect);
        painter->restore();
    }
}

void FlowChartExecutorItem::collidingDetect(QGraphicsScene *scene) {
    auto curRect = rect();
    while (true) {
        auto collidingItems = scene->collidingItems(this);
        for (int i = collidingItems.size() - 1; i >= 0; i--) {
            if (dynamic_cast<FlowChartExecutorItem*>(collidingItems[i]) == nullptr) {
                collidingItems.removeAt(i);
            }
        }
        if (collidingItems.isEmpty()) {
            break;
        }
        // 检查碰撞，并避免重叠
        for (auto item: collidingItems) {
            if (item != this && item->collidesWithItem(this)) {
                auto itemRect = item->boundingRect();
                if (curRect.intersects(itemRect)) {
                    curRect.moveTop(itemRect.bottom() + 1);
                    setRect(curRect);
                }
            }
        }
    }
}

void FlowChartExecutorItem::reCalcConnectPointPos() {
    connectPoints.clear();
    auto drawRect = rect().adjusted(5, 5, -5, -5);
    auto center = drawRect.center();
    connectPoints.append(QPointF(drawRect.left(), center.y())); //left
    connectPoints.append(QPointF(center.x(), drawRect.top())); //top
    connectPoints.append(QPointF(drawRect.right(), center.y())); //right
    connectPoints.append(QPointF(center.x(), drawRect.bottom())); //bottom
}

QPointF FlowChartExecutorItem::testInConnectPoint(const QPointF &mousePoint) {
    for (const auto& cp : connectPoints) {
        if (QRectF(cp.x() - 5, cp.y() - 5, 10, 10).contains(mousePoint)) {
            return cp;
        }
    }
    return QPointF();
}

int FlowChartExecutorItem::getConnectPointIndex(const QPointF &connectPoint) {
    return connectPoints.indexOf(connectPoint);
}

void FlowChartExecutorItem::setIsHoverState(bool isHover) {
    if (mouseHover != isHover) {
        mouseHover = isHover;
        update();
    }
}

void FlowChartExecutorItem::setConnectFromLine(FcConnectLine *connectLine) {
    for (int i = 0; i < connectPoints.size(); i++) {
        if (connectPoints[i] == connectLine->ctl.targetEnd) {
            connectFrom[i].append(connectLine);
            break;
        }
    }
}

void FlowChartExecutorItem::setConnectToLine(FcConnectLine *connectLine) {
    for (int i = 0; i < connectPoints.size(); i++) {
        if (connectPoints[i] == connectLine->ctl.targetBegin) {
            connectTo[i].append(connectLine);
            break;
        }
    }
}

void FlowChartExecutorItem::disconnectLine(FcConnectLine *line) {
    for (auto i = connectFrom.begin(); i != connectFrom.end(); ++i) {
        for (int j = 0; j < i.value().size(); j++) {
            if (line == i.value().at(j)) {
                i.value().removeAt(j);
                return;
            }
        }
    }
    for (auto i = connectTo.begin(); i != connectTo.end(); ++i) {
        for (int j = 0; j < i.value().size(); j++) {
            if (line == i.value().at(j)) {
                i.value().removeAt(j);
                return;
            }
        }
    }
}

void FlowChartExecutorItem::disconnectLines() {
    for (auto i = connectFrom.begin(); i != connectFrom.end(); ++i) {
        for (const auto& line : i.value()) {
            line->connectTo = nullptr;
        }
    }
    for (auto i = connectTo.begin(); i != connectTo.end(); ++i) {
        for (const auto& line : i.value()) {
            line->connectFrom = nullptr;
        }
    }
}

int FlowChartExecutorItem::getConnectedLineSize() const {
    int lineSize = 0;
    for (auto i = connectTo.begin(); i != connectTo.end(); i++) {
        lineSize += i.value().size();
    }
    return lineSize;
}

void FlowChartExecutorItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverEnterEvent(event);
    mouseHover = true;
}

void FlowChartExecutorItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverLeaveEvent(event);
    mouseHover = false;
}

void FlowChartExecutorItem::updateConnectLines() {

    for (int i = 0; i < connectPoints.size(); i++) {
        //test from
        {
            auto &lines = connectFrom[i];
            for (auto &line: lines) {
                line->ctl.targetEnd = connectPoints[i];
                line->ctl.updateControl();
                line->refreshConnectPath();
            }
        }
        //test to
        {
            auto &lines = connectTo[i];
            for (auto& line : lines) {
                line->ctl.targetBegin = connectPoints[i];
                line->ctl.updateControl();
                line->refreshConnectPath();
            }
        }
    }
}

void FlowChartExecutorItem::beginConnectLineMove() {
    for (int i = 0; i < connectPoints.size(); i++) {
        //test from
        {
            auto &lines = connectFrom[i];
            for (auto &line: lines) {
                line->ctl.beginMove();
            }
        }
        //test to
        {
            auto &lines = connectTo[i];
            for (auto& line : lines) {
                line->ctl.beginMove();
            }
        }
    }
}

