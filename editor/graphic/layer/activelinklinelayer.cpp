#include "activelinklinelayer.h"

ActiveLinkLineLayer::ActiveLinkLineLayer(QObject *parent)
    : GraphicLayer(parent)
{
}

void ActiveLinkLineLayer::reload(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing);

    for (const auto& linkLine : activeLinkLineList) {
        linkLine->graphicTransform = graphicTransform;
        linkLine->renderPainter = painter;
        linkLine->drawObject();
    }
}

void ActiveLinkLineLayer::reCache() {
}
