#pragma once

#include <qobject.h>

#include "../graphiccontrol.h"
#include "../graphicobject.h"
#include "../graphicobjecttype.h"

class GraphicObjCreateControl : public GraphicControl {
public:
    explicit GraphicObjCreateControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent = nullptr);

    void addObject(GraphicObjectType type, const QPoint& mousePoint);

private:
    GraphicObjectList graphicObjects;
    QSharedPointer<GraphicObject> activeObject;
};
