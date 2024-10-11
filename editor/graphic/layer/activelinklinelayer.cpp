#include "activelinklinelayer.h"

ActiveLinkLineLayer::ActiveLinkLineLayer(QObject *parent)
    : GraphicLayer(parent)
{
}

void ActiveLinkLineLayer::reCache() {
    layerCache.fill(Qt::transparent);

    QPainter painter(&layerCache);
    painter.setRenderHint(QPainter::Antialiasing);

    for (const auto& linkLine : activeLinkLineList) {
        linkLine->graphicTransform = graphicTransform;
        linkLine->drawGraphicObject(&painter, true);
    }
}
