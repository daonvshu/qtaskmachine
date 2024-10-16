#include "nodeeventstate.h"

NodeEventState::NodeEventState(const QSharedPointer<NodeEventStateData> &data)
    : GraphicNode(data)
    , eventStateData(data)
{
    data->propData.nodeName = "事件触发状态";
}
