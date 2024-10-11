#include "graphiclinkline.h"

GraphicLinkLine::GraphicLinkLine(const QSharedPointer<GraphicLinkLineData> &data)
    : GraphicObject(data)
    , linkData(data)
{
}

bool GraphicLinkLine::selectTest(const QPointF &point) {
    return GraphicObject::selectTest(point);
}
