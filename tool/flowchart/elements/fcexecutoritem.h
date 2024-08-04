#pragma once

#include "flowchartexecutoritem.h"

#include "../flowchartitemdata.h"

class FcExecutorItem : public FlowChartExecutorItem {
public:
    explicit FcExecutorItem(const FlowChartItemData& itemData, QGraphicsItem *parent = nullptr);

    static void drawItemBox(QPainter* painter, const QRectF& rect, FlowChartNodeType nodeType, const QString& text = QString());

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QSizeF itemSize() const override;

public:
    FlowChartItemData itemData;
};
