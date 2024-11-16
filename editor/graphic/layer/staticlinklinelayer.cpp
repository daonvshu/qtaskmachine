#include "staticlinklinelayer.h"

StaticLinkLineLayer::StaticLinkLineLayer(QObject *parent)
    : GraphicLayer(parent)
{
}

void StaticLinkLineLayer::reload(QPainter *painter) {
    if (graphicEntries == nullptr) {
        return;
    }

    for (int i = 0; i < graphicEntries->index(); i++) {
        auto linkLine = const_cast<GraphicLinkLine*>(dynamic_cast<const GraphicLinkLine*>(graphicEntries->command(i)));
        if (linkLine == nullptr) {
            continue;
        }
        if (linkLine->data->assignRemoved) {
            continue;
        }
        if (linkLine->linkData->selected) {
            continue;
        }
        if (linkLine->linkData->isEditing) {
            continue;
        }
        linkLine->graphicTransform = graphicTransform;
        linkLine->renderPainter = painter;
        linkLine->drawObject();
    }
}

void StaticLinkLineLayer::reCache() {
}
