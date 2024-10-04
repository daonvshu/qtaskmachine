#include "transform.h"

#include <qmath.h>
#include <qdebug.h>

GraphicTransform::GraphicTransform(QTransform transform)
    : transform(std::move(transform))
{}

GraphicTransform& GraphicTransform::operator=(const QTransform &trans) {
    this->transform = trans;
    return *this;
}

QPointF GraphicTransform::toGuiPoint(const QPointF &point) const {
    return transform.map(point);
}

QLineF GraphicTransform::toGuiPoint(const QLineF &line) const {
    return transform.map(line);
}

QRectF GraphicTransform::toGuiPoint(const QRectF &rect) const {
    return transform.map(rect).boundingRect();
}

qreal GraphicTransform::toGuiDx(qreal distance) const {
    return distance * std::hypot(transform.m11(), transform.m12());
}

qreal GraphicTransform::toGuiDy(qreal distance) const {
    return distance * std::hypot(transform.m22(), transform.m21());
}

qreal GraphicTransform::toRealDx(qreal distance) const {
    return distance / std::hypot(transform.m11(), transform.m12());
}

qreal GraphicTransform::toRealDy(qreal distance) const {
    return distance / std::hypot(transform.m22(), transform.m21());
}

QPointF GraphicTransform::toRealPoint(const QPointF &guiPoint) const {
    return transform.inverted().map(guiPoint);
}

QRectF GraphicTransform::toRealPoint(const QRectF &guiRect) const {
    return transform.inverted().mapRect(guiRect);
}
