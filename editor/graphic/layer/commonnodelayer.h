#pragma once

#include <qobject.h>

#include "../graphiclayer.h"
#include "../graphicobject.h"

class CommonNodeLayer : public GraphicLayer {
public:
    using GraphicLayer::GraphicLayer;

protected:
    void reCacheNodeObject(GraphicObject* object);

    void drawCache(const GraphicObject* object, QPainter* painter, bool drawSelectedBox = false);

    void drawActiveLinkPoint(const GraphicObject* object, QPainter* painter);
};
