#pragma once

#include "fciconitem.h"

class FcConditionalItem : public FcIconItem {
public:
    explicit FcConditionalItem(QGraphicsItem *parent = nullptr);

    int flowId = -1;

    static void drawItemBox(QPainter* painter, const QRectF& rect);

protected:
    QColor backgroundColor() override;
};