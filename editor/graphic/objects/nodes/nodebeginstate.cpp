#include "nodebeginstate.h"

NodeBeginState::NodeBeginState(const QSharedPointer<GraphicNodeData> &data)
    : GraphicNode(data)
{
    data->propData.nodeName = "开始";
    data->outputLinkPointColors << 0x00E0E0;
}
