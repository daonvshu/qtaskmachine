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

void NodeRecoveryState::fromExecutor(const ConfigFlowExecutor &executor) const {
    GraphicNode::fromExecutor(executor);
    recoveryStateData->recoveryPropData.nested = executor.nested();
}

void NodeRecoveryState::drawObject() {

    // calc min item width
    int minSubItemWidth = functionEnterWidth();
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight + propertyItemHeight;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Recovery_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    recoveryStateData->inputLinkPoints.clear();
    recoveryStateData->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    drawConnectableItem(itemEnterRow,
                        recoveryStateData->propData.funcEnter().isEmpty() ? "(onEnter)" : recoveryStateData->propData.funcEnter(),
                        itemFontSize, recoveryStateData->inputLinkPointColors.first(), true);

    // draw delay data row
    QRectF itemDataRow(bodyRect.left(), itemEnterRow.bottom(), bodyRect.width(), propertyItemHeight);
    drawPropertyRow(itemDataRow, recoveryStateData->recoveryPropData.nested() ? "嵌套" : "非嵌套", itemFontSize, recoveryStateData->inputLinkPointColors.first(), true);

    // draw property rows
    renderPropertyItems(bodyRect, itemDataRow.bottom(), bindStrings);

    GraphicRenderInterface::drawObject();
}
