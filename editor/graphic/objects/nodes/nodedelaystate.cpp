#include "nodedelaystate.h"

NodeDelayState::NodeDelayState(const QSharedPointer<NodeDelayStateData> &data)
    : GraphicNode(data)
    , delayStateData(data)
{
    data->propData.nodeName = "延时状态";
    data->inputLinkPointColors << 0x77E000;
    data->outputLinkPointColors << 0x00E0E0;
}
