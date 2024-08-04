#pragma once

#include <qobject.h>
#include <qpainterpath.h>

class CurveMoveControl : public QObject {
public:
    explicit CurveMoveControl(QObject *parent = nullptr);

    void beginMove();

    void updateControl();

    QPainterPath getOldPath();

    QPainterPath getCurrentPath();

public:
    //原始点
    QPointF targetBegin;
    QPointF targetEnd;
    QPointF targetControl;

    QPointF cachedRealCtlPoint;

    //记录点
    QPointF recordBegin, recordEnd, recordControl;

private:
    qreal footPointLengthRatio = 0; //垂足在线段上的比例位置
    bool getRatioPointByStart = false; //通过起点获取新垂足比例点
    qreal controlToLineDistance = 0; //控制点到线段的距离
    bool controlIsLeft = false; //控制点是否在线段左边
};
