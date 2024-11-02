#pragma once

#include <qobject.h>
#include <qsharedpointer.h>

#include "graphicobject.d.h"
#include "graphicobjecttype.h"
#include "graphicrender.h"

class GraphicObject
        : public GraphicRenderInterface
{
public:
    explicit GraphicObject(const QSharedPointer<GraphicObjectData>& data);

    virtual bool selectTest(const QPointF& point);

    virtual GraphicObjectType objectType() const = 0;

    virtual QSharedPointer<GraphicObject> clone() = 0;

    Q_DISABLE_COPY(GraphicObject);

public:
    QSharedPointer<GraphicObjectData> data;

protected:
    template<typename T, typename D, typename... Args>
    static QSharedPointer<T> objectCreate(Args... args) {
        return QSharedPointer<T>::create(QSharedPointer<D>::create(args...));
    }
};

typedef QList<QSharedPointer<GraphicObject>> GraphicObjectList;