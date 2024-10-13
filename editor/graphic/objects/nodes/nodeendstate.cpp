#include "nodeendstate.h"

NodeEndState::NodeEndState(const QSharedPointer<GraphicNodeData> &data)
    : GraphicNode(data)
{
    data->propData.nodeName = "结束";
}
