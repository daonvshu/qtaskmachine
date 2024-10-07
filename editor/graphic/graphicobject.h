#pragma once

#include <qobject.h>
#include <qsharedpointer.h>

#include "graphicobject.d.h"

#include "render/graphicrender.h"

class GraphicObject
        : public GraphicRenderInterface
{
public:
    explicit GraphicObject(const QSharedPointer<GraphicObjectData>& data);

    virtual bool selectTest(const QPointF& point);

    Q_DISABLE_COPY(GraphicObject);

public:
    QSharedPointer<GraphicObjectData> data;
};

typedef QList<QSharedPointer<GraphicObject>> GraphicObjectList;