#include "graphiclayer.h"

GraphicLayer::GraphicLayer(QObject *parent)
    : QObject(parent)
{
}

void GraphicLayer::sizeAdjust(const QSize &viewSize) {
    if (layerCache.isNull() || layerCache.size() != viewSize) {
        layerCache = QPixmap(viewSize);
        reCache();
    }
}