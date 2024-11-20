#include "staticlinklinelayer.h"

StaticLinkLineLayer::StaticLinkLineLayer(QObject *parent)
    : GraphicLayer(parent)
{
}

void StaticLinkLineLayer::reload(QPainter *painter) {
    if (graphicEntries == nullptr) {
        return;
    }

    auto linkLines = GraphicObject::getVisibleObjects<GraphicLinkLine>(graphicEntries);
    for (const auto& linkLine : linkLines) {
        if (linkLine->linkData->selected) {
            continue;
        }
        if (linkLine->linkData->isEditing) {
            continue;
        }
        const auto l = const_cast<GraphicLinkLine*>(linkLine);
        l->graphicTransform = graphicTransform;
        l->renderPainter = painter;
        l->drawObject();
    }
}

void StaticLinkLineLayer::reCache() {
}
