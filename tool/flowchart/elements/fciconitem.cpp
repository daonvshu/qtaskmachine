#include "fciconitem.h"

#include <qpainter.h>
#include <qstyleoption.h>
#include <qdebug.h>

FcIconItem::FcIconItem(const QString& iconUrl, QGraphicsItem *parent)
    : FlowChartExecutorItem(parent)
{
    iconPixmap = QPixmap(iconUrl);
}

void FcIconItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    drawItemBox(painter, option->rect, backgroundColor(), iconPixmap);
    FlowChartExecutorItem::paint(painter, option, widget);
}

void FcIconItem::drawItemBox(QPainter *painter, const QRectF &rect, const QColor &background, const QPixmap &icon) {
    auto drawRect = rect.adjusted(5, 5, -5, -5);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    //背景
    painter->setBrush(QBrush(background));
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(drawRect, 9, 9);
    //图标
    auto iconRect = icon.rect();
    iconRect.moveCenter(drawRect.center().toPoint());
    painter->drawPixmap(iconRect, icon);
}

QSizeF FcIconItem::itemSize() const {
    return {60, 32 + 11};
}

