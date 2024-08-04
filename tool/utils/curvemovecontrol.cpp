#include "curvemovecontrol.h"

#include "xmath.h"

CurveMoveControl::CurveMoveControl(QObject *parent)
    : QObject(parent)
{}

void CurveMoveControl::beginMove() {
    recordBegin = targetBegin;
    recordEnd = targetEnd;
    recordControl = targetControl;

    //计算控制点在线段上的垂足和距离
    auto oldVt = xmath::getMinDistancePoint(QLineF(recordBegin, recordEnd), recordControl, controlToLineDistance, false);
    //判断使用起点计算比例垂足还是终点，并且计算比例值
    if ((oldVt.x() >= recordBegin.x() && recordBegin.x() <= recordEnd.x()) ||
        (oldVt.x() <= recordBegin.x() && recordBegin.x() >= recordEnd.x()))
    {
        getRatioPointByStart = true;
        footPointLengthRatio = QLineF(oldVt, recordBegin).length() / QLineF(recordEnd, recordBegin).length();
    } else {
        getRatioPointByStart = false;
        footPointLengthRatio = QLineF(oldVt, recordEnd).length() / QLineF(recordEnd, recordBegin).length();
    }
    //判断控制点位置
    controlIsLeft = xmath::p2AbArea(recordBegin, recordEnd, recordControl) >= 0;
}

void CurveMoveControl::updateControl() {
    //新的垂足点
    auto vt = xmath::getPointByRatio(footPointLengthRatio, targetBegin, targetEnd, getRatioPointByStart);
    //平移垂足点到控制点
    targetControl = xmath::getParallelPoint(QLineF(targetBegin, targetEnd), vt, controlToLineDistance, controlIsLeft);
}

QPainterPath CurveMoveControl::getOldPath() {
    QPainterPath path;
    cachedRealCtlPoint = BezierUtil::getRealControlPoint(recordBegin, recordEnd, recordControl);
    path.moveTo(recordBegin);
    path.quadTo(cachedRealCtlPoint, recordEnd);
    return path;
}

QPainterPath CurveMoveControl::getCurrentPath() {
    QPainterPath path;
    cachedRealCtlPoint = BezierUtil::getRealControlPoint(targetBegin, targetEnd, targetControl);
    path.moveTo(targetBegin);
    path.quadTo(cachedRealCtlPoint, targetEnd);
    return path;
}
