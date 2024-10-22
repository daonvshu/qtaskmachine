/*
 * The code of this file is generated by o1-mini!
 */
#pragma once

#include <qobject.h>

struct BezierCurve {
    QPointF startPoint;
    QPointF controlPoint1;
    QPointF controlPoint2;
    QPointF endPoint;
};

class BezierDistanceCalculator {
public:
    // 静态函数：计算点到贝塞尔曲线的最小距离
    static qreal distanceToCurve(const QPointF &point, const BezierCurve &curve, int initialSamples = 100, int maxIterations = 10);

private:
    // 静态辅助函数
    static QPointF pointAt(const BezierCurve &curve, qreal t);
    static QPointF derivativeAt(const BezierCurve &curve, qreal t);
    static QPointF secondDerivativeAt(const BezierCurve &curve, qreal t);
    static qreal dotProduct(const QPointF &a, const QPointF &b);
    static qreal distanceSquared(const QPointF &a, const QPointF &b);
};
