#include "multiselectlayer.h"

MultiSelectLayer::MultiSelectLayer(QObject* parent)
    : GraphicLayer(parent)
{
}

void MultiSelectLayer::reload(QPainter* painter) {
    if (!selectRect.isValid()) {
        return;
    }
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);

    painter->setPen(0x888F9A);
    painter->setBrush(QColor("#1AFFFFFF"));

    painter->drawRect(selectRect);

    painter->restore();
}

void MultiSelectLayer::reCache() {
}

void MultiSelectLayer::beginSelect(const QPointF& mousePoint) {
    beginPoint = mousePoint;
}

void MultiSelectLayer::updateSelect(const QPointF& mousePoint) {
    selectRect = QRectF(QPointF(qMin(beginPoint.x(), mousePoint.x()), qMin(beginPoint.y(), mousePoint.y())),
        QPointF(qMax(beginPoint.x(), mousePoint.x()), qMax(beginPoint.y(), mousePoint.y())));
}

void MultiSelectLayer::endSelect() {
    selectRect = QRectF();
}
