#pragma once

#include <qobject.h>

#include "../graphiclayer.h"

/**
 * @brief 正交网格线
 */
class OrthogonalGridLayer : public GraphicLayer {
public:
    explicit OrthogonalGridLayer(QObject *parent = nullptr);

    void reload(QPainter *painter) override;

    void reCache() override;

    /**
     * @brief 捕捉测试
     * @param point
     */
    void snapTest(QPointF& point) const;

private:
    void drawGridLine(QPainter* painter, qreal width, const QColor& color);

public:
    bool gridEnabled;
    qreal gridWidth = 1; //网格间距，实际坐标系宽度
    QPixmap boxCache;

private:
    /**
     * @brief 副轴线颜色
     * @return
     */
    QColor gridColor() const;

    /**
     * @brief 主轴线颜色
     * @return
     */
    QColor metaGridColor() const;
};
