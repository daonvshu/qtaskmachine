#include "fcexecutoritem.h"

#include <qpainter.h>
#include <qstyleoption.h>
#include <qapplication.h>
#include <qdebug.h>

FcExecutorItem::FcExecutorItem(const FlowChartItemData &itemData, QGraphicsItem *parent)
    : FlowChartExecutorItem(parent)
    , itemData(itemData)
{
}

void FcExecutorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    drawItemBox(painter, option->exposedRect, itemData.nodeType, itemData.text);
    FlowChartExecutorItem::paint(painter, option, widget);
}

void FcExecutorItem::drawItemBox(QPainter *painter, const QRectF& rect, FlowChartNodeType nodeType, const QString& text) {
    painter->setRenderHint(QPainter::Antialiasing);
    auto drawRect = rect.adjusted(5, 5, -5, -5);
    //背景
    painter->setBrush(QBrush(0x9C27B0));
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(drawRect, 9, 9);
    //文字
    painter->setPen(Qt::white);
    painter->drawText(drawRect, Qt::AlignCenter, text);
}

QSizeF FcExecutorItem::itemSize() const {
    auto fm = qApp->fontMetrics();
    auto textWidth = fm.horizontalAdvance(itemData.text);
    return { qMax(60.0, qreal(textWidth + 18 + 10)), 32 + 10 };
}
