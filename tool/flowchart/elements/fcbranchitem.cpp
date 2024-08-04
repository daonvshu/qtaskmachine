#include "fcbranchitem.h"

#include <qpainter.h>
#include <qstyleoption.h>
#include <qdebug.h>

FcConditionalItem::FcConditionalItem(QGraphicsItem *parent)
    : FcIconItem(":/res/conditional.svg", parent)
{}

QColor FcConditionalItem::backgroundColor() {
    return 0xFF8C00;
}

void FcConditionalItem::drawItemBox(QPainter *painter, const QRectF &rect) {
    FcIconItem::drawItemBox(painter, rect, 0xFF8C00, QPixmap(":/res/conditional.svg"));
}
