#pragma once

#include <qobject.h>

#include "../graphiclayer.h"
#include "../graphicobject.h"

class CommonNodeLayer : public GraphicLayer {
public:
    using GraphicLayer::GraphicLayer;

protected:
    void reCacheNodeObject(const QSharedPointer<GraphicObject>& object);

    void drawCache(const QSharedPointer<GraphicObject>& object, QPainter* painter, bool drawSelectedBox = false);

    void drawActiveLinkPoint(const QSharedPointer<GraphicObject>& object, QPainter* painter);
};
