#pragma once

#include "../graphicnode.d.h"

#include "data/recoverystatepropdata.h"

struct NodeRecoveryStateData : GraphicNodeData {
    RecoveryStatePropertyData recoveryPropData;
};