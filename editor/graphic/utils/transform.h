#pragma once

#include <qobject.h>
#include <qtransform.h>

class GraphicTransform {
public:
    GraphicTransform() = default;
    explicit GraphicTransform(QTransform transform);
    GraphicTransform& operator=(const QTransform& trans);

    /**
     * @brief 实际坐标系点转绘图坐标系点
     * @param point
     * @return
     */
    QPointF toGuiPoint(const QPointF& point) const;

    /**
     * @brief 实际坐标系点转绘图坐标系点
     * @param point
     * @return
     */
    QLineF toGuiPoint(const QLineF& line) const;

    /**
     * @brief 实际坐标系点转绘图坐标系点
     * @param rect
     * @return
     */
    QRectF toGuiPoint(const QRectF& rect) const;

    /**
     * @brief 实际水平X轴距离转绘图坐标系像素距离
     * @param distance
     */
    qreal toGuiDx(qreal distance) const;

    /**
     * @brief 实际垂直Y轴距离转绘图坐标系像素距离，目前dx缩放比等于dy缩放比
     * @param distance
     */
    qreal toGuiDy(qreal distance) const;

    /**
     * @brief 视图像素距离转实际距离，水平X轴方向
     * @param distance
     * @return
     */
    qreal toRealDx(qreal distance) const;

    /**
     * @brief 视图像素距离转实际距离，水平Y轴方向
     * @param distance
     * @return
     */
    qreal toRealDy(qreal distance) const;

    /**
     * @brief 绘图坐标系点转实际坐标系点
     * @param guiPoint
     * @return
     */
    QPointF toRealPoint(const QPointF& guiPoint) const;

    /**
     * @brief 绘图坐标系矩形转实际坐标系坐标点
     * @param guiRect
     * @return
     */
    QRectF toRealPoint(const QRectF& guiRect) const;

    QTransform getTransform() const {
        return transform;
    }

private:
    QTransform transform;
};
