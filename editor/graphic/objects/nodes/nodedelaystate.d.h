#pragma once

#include "../graphicnode.d.h"

#include "data/delaystatepropertydata.h"

struct NodeDelayStateData : GraphicNodeData {
    DelayStatePropertyData delayPropData;
};