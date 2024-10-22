#include "noderecoverystate.h"

NodeRecoveryState::NodeRecoveryState(const QSharedPointer<NodeRecoveryStateData> &data)
    : GraphicNode(data)
    , recoveryStateData(data)
{
    data->propData.nodeName = "恢复点";
    data->inputLinkPointColors << 0x77E000;
}
