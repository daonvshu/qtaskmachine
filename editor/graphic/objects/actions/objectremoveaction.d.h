#pragma once

#include "../../graphicobject.h"

struct ObjectRemoveActionData : GraphicObjectData {
    QList<const GraphicObject*> targets;

    explicit ObjectRemoveActionData(const QList<const GraphicObject*>& targets): targets(targets) {}
};