#include "staticlinklinelayer.h"

StaticLinkLineLayer::StaticLinkLineLayer(QObject *parent)
    : GraphicLayer(parent)
{
}

void StaticLinkLineLayer::reCache() {
    layerCache.fill(Qt::transparent);

    QPainter painter(&layerCache);
    painter.setRenderHint(QPainter::Antialiasing);

    for (const auto& node : staticLinkLineList) {
        if (node->linkData->selected) {
            continue;
        }
        if (node->linkData->isEditing) {
            continue;
        }
        node->graphicTransform = graphicTransform;
        node->drawGraphicObject(&painter, false);
    }
}
