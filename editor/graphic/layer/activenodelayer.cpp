#include "activenodelayer.h"

ActiveNodeLayer::ActiveNodeLayer(QObject *parent)
    : GraphicLayer(parent)
{
}

void ActiveNodeLayer::reCache() {
    layerCache.fill(Qt::transparent);

    if (activeNode.isNull()) {
        return;
    }

    QPainter painter(&layerCache);
    painter.setRenderHint(QPainter::Antialiasing);

    activeNode->graphicTransform = graphicTransform;
    activeNode->drawGraphicObject(&painter, true);
}
