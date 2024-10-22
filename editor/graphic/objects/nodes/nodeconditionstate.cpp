#include "nodeconditionstate.h"

NodeConditionState::NodeConditionState(const QSharedPointer<NodeConditionStateData>& data)
    : GraphicNode(data)
    , conditionStateData(data)
{
    data->propData.nodeName = "条件分支状态";
    data->inputLinkPointColors << 0x77E000;
    data->outputLinkPointColors << 0x00E0E0;
}
