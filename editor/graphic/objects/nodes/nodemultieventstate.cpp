#include "nodemultieventstate.h"

NodeMultiEventState::NodeMultiEventState(const QSharedPointer<NodeMultiEventStateData> &data)
    : GraphicNode(data)
    , eventStateData(data)
{
    data->propData.nodeName = "复合事件状态";
}
