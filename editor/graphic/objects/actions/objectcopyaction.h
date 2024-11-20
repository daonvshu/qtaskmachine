#pragma once

#include <qobject.h>

#include "objectcopyaction.d.h"

class ObjectCopyAction : public GraphicObject {
public:
    explicit ObjectCopyAction(const QSharedPointer<ObjectCopyActionData>& data);

    static ObjectCopyAction* create(const QList<const GraphicObject*>& targets) {
        return objectCreate<ObjectCopyAction, ObjectCopyActionData>(targets);
    }

    GraphicObject * clone() const override {
        return objectCreate<ObjectCopyAction, ObjectCopyActionData>(*actionData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Action_Object_Copy;
    }

    void undo() override;

    void redo() override;

public:
    QSharedPointer<ObjectCopyActionData> actionData;
};
