#include "graphicobject.h"

#include <styledstring.h>

class ObjectMemoryLeakDetector {
public:
    static ObjectMemoryLeakDetector instance;

    int objectCount = 0;

    ~ObjectMemoryLeakDetector() {
        if (objectCount != 0) {
            qCritical() << styled("GraphicObject memory leak detected, object count:").r() << objectCount;
        }
    }
};

ObjectMemoryLeakDetector ObjectMemoryLeakDetector::instance;

GraphicObject::GraphicObject(const QSharedPointer<GraphicObjectData> &data)
    : data(data)
{
    ObjectMemoryLeakDetector::instance.objectCount++;
}

GraphicObject::~GraphicObject() {
    ObjectMemoryLeakDetector::instance.objectCount--;
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
