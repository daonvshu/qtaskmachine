#include "graphicobject.h"

GraphicObject::GraphicObject(const QSharedPointer<GraphicObjectData> &data)
    : data(data)
{

}

bool GraphicObject::selectTest(const QPointF &point) const {
    return false;
}

void GraphicObject::undo() {
    data->assignRemoved = true;
}

void GraphicObject::redo() {
    data->assignRemoved = false;
}
