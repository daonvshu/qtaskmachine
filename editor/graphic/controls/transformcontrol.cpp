#include "transformcontrol.h"

#include <qdebug.h>

TransformControl::TransformControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent)
    : GraphicControl(data, parent)
{}

void TransformControl::coordinateReload() {
    auto viewCenter = d->view->rect().center();
    d->coordinateTransform = QTransform();
    d->coordinateTransform.translate(viewCenter.x(), viewCenter.y());
}

void TransformControl::moveBegin(const QPoint &mousePoint) {
    lastMoveBeginPos = mousePoint;
}

void TransformControl::dragMoving(const QPoint &mousePoint) {
    auto delta = mousePoint - lastMoveBeginPos;

    QTransform translation;
    translation.translate(delta.x(), delta.y());
    d->graphicTransform *= translation;
    lastMoveBeginPos = mousePoint;
}

//consider use snapped mouse point!
bool TransformControl::scale(bool zoomIn, const QPointF& mousePoint) {
    const qreal zoomFactor = 0.137;

    qreal scaleFactor = zoomIn ? (1 - zoomFactor) : (1 + zoomFactor);
    //测试缩放之后的坐标系大小
    auto toScaleFactor = scaleFactor * d->graphicTransform.m11();
    if (toScaleFactor <= 0.1 || toScaleFactor >= 5.0) {
        return false;
    }
    //qDebug() << "apply scale factor:" << toScaleFactor;

    // 应用缩放变换
    QTransform transform;
    transform.translate(mousePoint.x(), mousePoint.y());
    transform.scale(scaleFactor, scaleFactor);
    transform.translate(-mousePoint.x(), -mousePoint.y());
    d->graphicTransform *= transform;

    return true;
}

void TransformControl::resetTransform(qreal offsetX, qreal offsetY) const {
    QPointF realCenter(offsetX, offsetY);
    auto targetCenter = d->getGraphicTransform().toGuiPoint(realCenter);
    auto currentCenter = d->view->rect().center();
    auto delta = currentCenter - targetCenter;
    QTransform translation;
    translation.translate(delta.x(), delta.y());
    d->graphicTransform *= translation;
}
