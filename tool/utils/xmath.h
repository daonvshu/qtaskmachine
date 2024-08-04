#pragma once

#include <qline.h>
#include <qmath.h>

namespace xmath {
    //求矢量pp1与pp2的面积s，s>0：p在矢量p1p2左侧，pp1p2逆时针旋转，s<0: p在矢量p1p2右侧，pp1p2顺时针旋转，s=0：p在p1p2上
    extern qreal p2AbArea(QPointF p1, QPointF p2, QPointF p);

    //求线段line上的点pointAtLine向left方向平移r个距离的点
    extern QPointF getParallelPoint(const QLineF& line, const QPointF& pointAtLine, qreal r, bool left);
    //获取点到直线垂足
    extern QPointF getFootPoint(const QPointF& p, const QPointF& p1, const QPointF& p2);

    //矢量点乘，dot>0：锐角，dot=0：直角，dot<0：钝角
    extern qreal dotProduct(const QLineF& beginLine, const QLineF& endLine);

    //获取点point到线段line上的最短距离和对应点
    extern QPointF getMinDistancePoint(const QLineF& line, const QPointF& point, qreal& distance, bool limitInLine = true);

    //获取从p1到p2(useP1=true)在比例位置ratio的点
    extern QPointF getPointByRatio(qreal ratio, QPointF p1, QPointF p2, bool useP1);
}

//https://pomax.github.io/bezierinfo/zh-CN/index.html#pointcurves
namespace BezierUtil {
    extern qreal projectionRatio(qreal t, int n);

    extern qreal abcRatio(qreal t, int n);

    extern qreal getT(const QPointF &begin, const QPointF &end, const QPointF &control);

    extern QPointF getRealControlPoint(const QPointF &begin, const QPointF &end, const QPointF &control);
}