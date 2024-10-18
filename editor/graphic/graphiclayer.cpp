#include "graphiclayer.h"

GraphicLayer::GraphicLayer(QObject *parent)
    : QObject(parent)
{
}

bool GraphicLayer::sizeAdjust(const QSize &viewSize) {
    if (layerCache.isNull() || layerCache.size() != viewSize) {
        layerCache = QPixmap(viewSize);
        reCache();
        return true;
    }
    return false;
}