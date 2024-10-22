#include "bezierdistancecalculator.h"

#include <qmath.h>

// 计算点在参数 t 处的曲线上的坐标
QPointF BezierDistanceCalculator::pointAt(const BezierCurve &curve, qreal t) {
    qreal oneMinusT = 1.0 - t;
    return oneMinusT * oneMinusT * oneMinusT * curve.startPoint
           + 3.0 * oneMinusT * oneMinusT * t * curve.controlPoint1
           + 3.0 * oneMinusT * t * t * curve.controlPoint2
           + t * t * t * curve.endPoint;
}

// 计算曲线在参数 t 处的一阶导数 B'(t)
QPointF BezierDistanceCalculator::derivativeAt(const BezierCurve &curve, qreal t) {
    qreal oneMinusT = 1.0 - t;
    return 3.0 * oneMinusT * oneMinusT * (curve.controlPoint1 - curve.startPoint)
           + 6.0 * oneMinusT * t * (curve.controlPoint2 - curve.controlPoint1)
           + 3.0 * t * t * (curve.endPoint - curve.controlPoint2);
}

// 计算曲线在参数 t 处的二阶导数 B''(t)
QPointF BezierDistanceCalculator::secondDerivativeAt(const BezierCurve &curve, qreal t) {
    qreal oneMinusT = 1.0 - t;
    return 6.0 * oneMinusT * (curve.controlPoint2 - 2.0 * curve.controlPoint1 + curve.startPoint)
           + 6.0 * t * (curve.endPoint - 3.0 * curve.controlPoint2 + 3.0 * curve.controlPoint1 - curve.startPoint);
}

// 计算两个向量的点积
qreal BezierDistanceCalculator::dotProduct(const QPointF &a, const QPointF &b) {
    return a.x() * b.x() + a.y() * b.y();
}

// 计算两个点之间的平方距离
qreal BezierDistanceCalculator::distanceSquared(const QPointF &a, const QPointF &b) {
    qreal dx = a.x() - b.x();
    qreal dy = a.y() - b.y();
    return dx * dx + dy * dy;
}

// 计算点到曲线的最小距离
qreal BezierDistanceCalculator::distanceToCurve(const QPointF &point, const BezierCurve &curve, int initialSamples, int maxIterations) {
    // 初始采样
    qreal minDistSquared = std::numeric_limits<qreal>::max();
    qreal bestT = 0.0;
    qreal step = 1.0 / initialSamples;

    for (int i = 0; i <= initialSamples; ++i) {
        qreal t = i * step;
        QPointF Bt = pointAt(curve, t);
        qreal distSq = distanceSquared(Bt, point);
        if (distSq < minDistSquared) {
            minDistSquared = distSq;
            bestT = t;
        }
    }

    // 牛顿-拉弗森迭代法
    for (int iter = 0; iter < maxIterations; ++iter) {
        QPointF Bt = pointAt(curve, bestT);
        QPointF Bprime = derivativeAt(curve, bestT);
        QPointF diff = Bt - point;
        qreal f = dotProduct(diff, Bprime);
        QPointF BdoublePrime = secondDerivativeAt(curve, bestT);
        qreal f_prime = dotProduct(Bprime, Bprime) + dotProduct(diff, BdoublePrime);

        if (qFuzzyCompare(f_prime, qreal(0.0))) {
            break;
        }

        qreal t_new = bestT - f / f_prime;

        if (qAbs(t_new - bestT) < 1e-6) {
            bestT = t_new;
            break;
        }

        bestT = qBound(qreal(0.0), t_new, qreal(1.0));
    }

    // 计算最终距离
    QPointF bestPoint = pointAt(curve, bestT);
    qreal finalDist = qSqrt(distanceSquared(bestPoint, point));
    return finalDist;
}