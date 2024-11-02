#include "nodeendstate.h"

NodeEndState::NodeEndState(const QSharedPointer<GraphicNodeData> &data)
    : GraphicNode(data)
{
    data->propData.nodeName = "结束";
    data->inputLinkPointColors << 0x77E000;
}

void NodeEndState::drawObject() {

    // calc min item width
    int minSubItemWidth = functionEnterWidth();
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
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_End_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    nodeData->inputLinkPoints.clear();
    nodeData->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    drawConnectableItem(itemEnterRow,
                        nodeData->propData.funcEnter().isEmpty() ? "(onEnter)" : nodeData->propData.funcEnter(),
                        itemFontSize, nodeData->inputLinkPointColors.first(), true);

    // draw property rows
    renderPropertyItems(bodyRect, itemEnterRow.bottom(), bindStrings);

    GraphicRenderInterface::drawObject();
}
