#pragma once

#include "../../graphicobject.d.h"

#include "../graphicnode.h"

struct NodeMoveActionData : GraphicObjectData {
    const GraphicNode* target = nullptr;

    explicit NodeMoveActionData(const GraphicNode* target): target(target) {}
};