#include "nodedelaystate.h"

NodeDelayState::NodeDelayState(const QSharedPointer<NodeDelayStateData> &data)
    : GraphicNode(data)
    , delayStateData(data)
{
    data->propData.nodeName = "延时状态";
}
