#include "fcconnectline.h"

#include <qmath.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <QGraphicsSceneMouseEvent>
#include <qstyleoption.h>

FcConnectLine::FcConnectLine(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    setZValue(0);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}

void FcConnectLine::refreshConnectPath(bool updateControlPoint) {
    if (updateControlPoint) {
        ctl.targetControl = QLineF(ctl.targetBegin, ctl.targetEnd).center();
    }
    setPath(ctl.getCurrentPath());
    update();
}

void FcConnectLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(0xFF8C00);
    painter->drawPath(path());

    if (option->state.testFlag(QStyle::State_Selected)) {
        painter->save();
        auto pen = painter->pen();
        pen.setColor(0x026EC1);
        pen.setDashPattern({3, 2, 3, 2});
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawPath(path());
        painter->restore();
    }

    if (isHover) {
        painter->setBrush(QBrush(0xFF8C00));
        painter->drawEllipse(ctl.targetControl, 4, 4);
    }

    const auto& endPoint = ctl.targetEnd;
    //draw arrow
    double angle = atan2(endPoint.y() - ctl.cachedRealCtlPoint.y(), 
                         endPoint.x() - ctl.cachedRealCtlPoint.x());
    double arrowAngle = 20 * M_PI / 180; // 将角度转换为弧度
    int arrowLength = 6;

    int arrowPointX = qRound(endPoint.x() - arrowLength * cos(angle - arrowAngle));
    int arrowPointY = qRound(endPoint.y() - arrowLength * sin(angle - arrowAngle));
    painter->drawLine(endPoint, QPoint(arrowPointX, arrowPointY));

    arrowPointX = qRound(endPoint.x() - arrowLength * cos(angle + arrowAngle));
    arrowPointY = qRound(endPoint.y() - arrowLength * sin(angle + arrowAngle));
    painter->drawLine(endPoint, QPoint(arrowPointX, arrowPointY));
}

QRectF FcConnectLine::boundingRect() const {
    return QGraphicsPathItem::boundingRect().adjusted(-20, -20, 20, 20);
}

void FcConnectLine::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        auto ctlRect = QRectF(ctl.targetControl.x() - 10, ctl.targetControl.y() - 10, 21, 21);
        if (ctlRect.contains(event->pos())) {
            isMoving = true;
            offset = ctl.targetControl - event->pos();
            setZValue(1);
        }
    }
    QGraphicsPathItem::mousePressEvent(event);
}

void FcConnectLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isMoving) {
        ctl.targetControl = event->pos() - offset;
        refreshConnectPath();
    }
    QGraphicsPathItem::mouseMoveEvent(event);
}

void FcConnectLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    isMoving = false;
    setZValue(0);
    QGraphicsPathItem::mouseReleaseEvent(event);
}

void FcConnectLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverEnterEvent(event);
    isHover = true;
}

void FcConnectLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverLeaveEvent(event);
    isHover = false;
}