#pragma once

#include <qobject.h>
#include <qpainter.h>

#include "utils/transform.h"

class GraphicLayer : public QObject {
public:
    explicit GraphicLayer(QObject *parent = nullptr);

    /**
     * @brief 重绘图层
     */
    virtual void reload(QPainter* painter) = 0;

    /**
     * @brief 重置缓存
     */
    virtual void reCache() = 0;

    bool checkAndUpdateScale();

public:
    QSize viewSize;
    GraphicTransform graphicTransform; //! 坐标变换，视图移动、缩放之后会改变变换器参数
    qreal oldScaleRatio = 0;
};
