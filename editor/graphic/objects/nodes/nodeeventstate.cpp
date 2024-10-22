#include "nodeeventstate.h"

NodeEventState::NodeEventState(const QSharedPointer<NodeEventStateData> &data)
    : GraphicNode(data)
    , eventStateData(data)
{
    data->propData.nodeName = "事件触发状态";
    data->inputLinkPointColors << 0x77E000;
    data->outputLinkPointColors << 0x00E0E0 << 0xD81E06;
}
