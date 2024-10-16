#pragma once

#include "../graphicnode.d.h"

#include "data/conditionstatepropdata.h"

struct NodeConditionStateData : GraphicNodeData {
    ConditionStatePropertyData conditionPropData;
};