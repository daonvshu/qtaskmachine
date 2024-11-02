#include "graphiclayer.h"

GraphicLayer::GraphicLayer(QObject *parent)
    : QObject(parent)
{
}

bool GraphicLayer::checkAndUpdateScale() {
    auto currentScale = graphicTransform.getTransform().m11();
    if (oldScaleRatio != currentScale) {
        oldScaleRatio = currentScale;
        return true;
    }
    return false;
}
