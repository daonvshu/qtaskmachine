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

    template<typename T, typename D, typename... Args>
    static T* objectClone(const QSharedPointer<D>& data, Args... args) {
        return new T(QSharedPointer<D>::create(*data), args...);
    }

public:
    template<typename T>
    static QList<const T*> getVisibleObjects(QUndoStack* objects) {
        QList<const T*> result;
        auto appendObject = [&] (const QUndoCommand* command) {
            auto object = dynamic_cast<const T*>(command);
            if (object == nullptr) {
                return;
            }
            if (object->data->assignRemoved) {
                return;
            }
            result.append(object);
        };
        for (int i = 0; i < objects->index(); i++) {
            auto command = objects->command(i);
            if (command->childCount() != 0) {
                for (int j = 0; j < command->childCount(); j++) {
                    appendObject(command->child(j));
                }
            } else {
                appendObject(command);
            }
        }
        return result;
    }
};

typedef QList<QSharedPointer<GraphicObject>> GraphicObjectList;