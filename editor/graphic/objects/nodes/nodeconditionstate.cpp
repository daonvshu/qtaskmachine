#include "nodeconditionstate.h"

NodeConditionState::NodeConditionState(const QSharedPointer<NodeConditionStateData>& data)
    : GraphicNode(data)
    , conditionStateData(data)
{
    data->propData.nodeName = "条件分支状态";
    data->inputLinkPointColors << 0x77E000;
    data->outputLinkPointColors << 0x00E0E0;
}

ConfigFlowExecutor NodeConditionState::toFlowExecutor() const {
    auto executor = GraphicNode::toFlowExecutor();
    executor.condition = conditionStateData->conditionPropData.conditionFunc();

    return executor;
}

void NodeConditionState::fromExecutor(const ConfigFlowExecutor &executor) {
    GraphicNode::fromExecutor(executor);
    conditionStateData->conditionPropData.conditionFunc = executor.condition();
    conditionStateData->conditionPropData.branchIds().clear();
}
