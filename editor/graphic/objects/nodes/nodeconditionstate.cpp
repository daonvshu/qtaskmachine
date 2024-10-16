#include "nodeconditionstate.h"

NodeConditionState::NodeConditionState(const QSharedPointer<NodeConditionStateData>& data)
    : GraphicNode(data)
    , conditionStateData(data)
{
    data->propData.nodeName = "条件分支状态";
}
