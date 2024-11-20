#pragma once

#include <qobject.h>

#include "objectremoveaction.d.h"

class ObjectRemoveAction : public GraphicObject {
public:
    explicit ObjectRemoveAction(const QSharedPointer<ObjectRemoveActionData>& data);

    static ObjectRemoveAction* create(const QList<const GraphicObject*>& targets) {
        return objectCreate<ObjectRemoveAction, ObjectRemoveActionData>(targets);
    }

    static ObjectRemoveAction* create(const GraphicObject* target) {
        return objectCreate<ObjectRemoveAction, ObjectRemoveActionData>(QList { target });
    }

    GraphicObject * clone() const override {
        return objectCreate<ObjectRemoveAction, ObjectRemoveActionData>(*actionData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Action_Object_Remove;
    }

    void undo() override;

    void redo() override;

public:
    QSharedPointer<ObjectRemoveActionData> actionData;
};
