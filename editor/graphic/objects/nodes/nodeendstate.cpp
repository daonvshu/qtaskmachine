#include "nodeendstate.h"

NodeEndState::NodeEndState(const QSharedPointer<GraphicNodeData> &data)
    : GraphicNode(data)
{
    data->propData.nodeName = "结束";
    data->inputLinkPointColors << 0x77E000;
}
