#include "nodebeginstate.h"

NodeBeginState::NodeBeginState(const QSharedPointer<GraphicNodeData> &data)
    : GraphicNode(data)
{
    data->propData.nodeName = "开始";
    data->outputLinkPointColors << 0x00E0E0;
}

void NodeBeginState::drawObject() {
    // calc min item width
    int minSubItemWidth = functionExitWidth();
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Begin_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemExitRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    nodeData->outputLinkPoints.clear();
    nodeData->outputLinkPoints << getConnectPointRect(itemExitRow, false);
    drawConnectableItem(itemExitRow,
                        nodeData->propData.funcExit().isEmpty() ? "(onExit)" : nodeData->propData.funcExit(),
                        itemFontSize, nodeData->outputLinkPointColors.first(), false);

    // draw property rows
    renderPropertyItems(bodyRect, itemExitRow.bottom(), bindStrings);

    GraphicRenderInterface::drawObject();
}
