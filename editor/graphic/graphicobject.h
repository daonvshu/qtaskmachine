#pragma once

#include <qobject.h>
#include <qsharedpointer.h>
#include <qundostack.h>

#include "graphicobject.d.h"
#include "graphicobjecttype.h"
#include "graphicrender.h"

class GraphicObject
    : public GraphicRenderInterface
    , public QUndoCommand
{
public:
    explicit GraphicObject(const QSharedPointer<GraphicObjectData>& data);
    ~GraphicObject();

    virtual bool selectTest(const QPointF& point) const;

    virtual GraphicObjectType objectType() const = 0;

    virtual GraphicObject* clone() const = 0;

    Q_DISABLE_COPY(GraphicObject);

    void undo() override;

    void redo() override;

public:
    QSharedPointer<GraphicObjectData> data;

protected:
    template<typename T, typename D, typename... Args>
    static T* objectCreate(Args... args) {
        return new T(QSharedPointer<D>::create(args...));
    }
};

typedef QList<QSharedPointer<GraphicObject>> GraphicObjectList;