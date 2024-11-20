#pragma once

#include "../../graphicobject.d.h"

#include "../graphicnode.h"

struct ObjectCopyActionData : GraphicObjectData {
    QList<const GraphicObject*> targets;

    explicit ObjectCopyActionData(const QList<const GraphicObject*>& targets): targets(targets) {}
};