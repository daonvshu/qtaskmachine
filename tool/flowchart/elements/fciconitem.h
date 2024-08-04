#pragma once

#include "flowchartexecutoritem.h"

class FcIconItem : public FlowChartExecutorItem {
public:
    explicit FcIconItem(const QString& iconUrl, QGraphicsItem *parent = nullptr);

    static void drawItemBox(QPainter* painter, const QRectF& rect, const QColor& background, const QPixmap& icon);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QSizeF itemSize() const override;

    virtual QColor backgroundColor() = 0;

private:
    QPixmap iconPixmap;
};
