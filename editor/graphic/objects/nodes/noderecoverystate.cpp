#include "noderecoverystate.h"

NodeRecoveryState::NodeRecoveryState(const QSharedPointer<NodeRecoveryStateData> &data)
    : GraphicNode(data)
    , recoveryStateData(data)
{
    data->propData.nodeName = "恢复点";
    data->inputLinkPointColors << 0x77E000;
}

ConfigFlowExecutor NodeRecoveryState::toFlowExecutor() const {
    auto executor = GraphicNode::toFlowExecutor();
    executor.nested = recoveryStateData->recoveryPropData.nested();

    return executor;
}

void NodeRecoveryState::fromExecutor(const ConfigFlowExecutor &executor) {
    GraphicNode::fromExecutor(executor);
    recoveryStateData->recoveryPropData.nested = executor.nested();
}
