#include "graphicobject.h"

GraphicObject::GraphicObject(const QSharedPointer<GraphicObjectData> &data)
    : data(data)
{

}

bool GraphicObject::selectTest(const QPointF &point) {
    return false;
}
