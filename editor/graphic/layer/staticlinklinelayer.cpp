#include "staticlinklinelayer.h"

StaticLinkLineLayer::StaticLinkLineLayer(QObject *parent)
    : GraphicLayer(parent)
{
}

void StaticLinkLineLayer::reload(QPainter *painter) {
    for (const auto& node : staticLinkLineList) {
        if (node->linkData->selected) {
            continue;
        }
        if (node->linkData->isEditing) {
            continue;
        }
        node->graphicTransform = graphicTransform;
        node->renderPainter = painter;
        node->drawObject();
    }
}

void StaticLinkLineLayer::reCache() {
}
