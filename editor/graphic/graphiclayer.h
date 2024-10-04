#pragma once

#include <qobject.h>
#include <qpainter.h>

#include "utils/transform.h"

class GraphicLayer : public QObject {
public:
    explicit GraphicLayer(QObject *parent = nullptr);

    /**
     * @brief 根据视图调整缓存大小
     * @param viewSize
     */
    void sizeAdjust(const QSize& viewSize);

    /**
     * @brief 重置缓存
     */
    virtual void recache() = 0;

public:
    QPixmap layerCache; //! 双缓冲图形层
    GraphicTransform graphicTransform; //! 坐标变换，视图移动、缩放之后会改变变换器参数
};
