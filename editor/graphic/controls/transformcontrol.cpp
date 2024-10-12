#include "transformcontrol.h"

#include <qdebug.h>

TransformControl::TransformControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent)
    : GraphicControl(data, parent)
{}

void TransformControl::scaleByDefault() {
    if (!isFirstLoad) {
        return;
    }
    isFirstLoad = false;

    auto viewCenter = d->view->rect().center();
    //QTransform transform;
    //transform.translate(viewCenter.x(), viewCenter.y());
    //transform.scale(10.0, 10.0);
    //transform.translate(-viewCenter.x(), -viewCenter.y());
    //d->graphicTransform *= transform;

    oldViewCenter = viewCenter;
}

void TransformControl::coordinateReload() {
    auto oldCenterRel = d->getGraphicTransform().toRealPoint(oldViewCenter); //原先界面中心位置的实际坐标值
    auto viewCenter = d->view->rect().center();
    d->coordinateTransform = QTransform();
    d->coordinateTransform.translate(viewCenter.x(), viewCenter.y());
    //笛卡尔坐标系变换
    //d->coordinateTransform.scale(1, -1);

    if (!isFirstLoad) {
        //视图中心坐标对应的实际值相对当前视图中心产生了偏移量
        auto oldCenterGui = d->getGraphicTransform().toGuiPoint(oldCenterRel);
        auto delta = oldCenterGui - viewCenter;
        //移动偏移量，保持实际坐标中心值在视图中心
        QTransform transform;
        transform.translate(-delta.x(), -delta.y());
        d->graphicTransform *= transform;
        //更新视图中心
        oldViewCenter = viewCenter;
    }
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

void TransformControl::applyTransform(const QTransform &transform) {
    d->graphicTransform *= transform;
}
