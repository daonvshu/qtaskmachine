#pragma once

#include "../../graphicobject.h"

struct ObjectRemoveActionData : GraphicObjectData {
    const GraphicObject* target = nullptr;

    explicit ObjectRemoveActionData(const GraphicObject* target): target(target) {}
};