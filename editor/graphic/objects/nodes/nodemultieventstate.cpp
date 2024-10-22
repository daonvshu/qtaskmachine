#include "nodemultieventstate.h"

NodeMultiEventState::NodeMultiEventState(const QSharedPointer<NodeMultiEventStateData> &data)
    : GraphicNode(data)
    , eventStateData(data)
{
    data->propData.nodeName = "复合事件状态";
    data->inputLinkPointColors << 0x77E000;
    data->outputLinkPointColors << 0x00E0E0;
}
