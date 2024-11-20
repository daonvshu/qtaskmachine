#include "nodenormalstate.h"

NodeNormalState::NodeNormalState(const QSharedPointer<GraphicNodeData> &data, bool initialData)
    : GraphicNode(data)
{
    if (initialData) {
        data->propData.nodeName = "普通状态";
        data->inputLinkPointColors << 0x77E000;
        data->outputLinkPointColors << 0x00E0E0;
    }
}

void NodeNormalState::drawObject() {

    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * 2;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Normal_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    nodeData->inputLinkPoints.clear();
    nodeData->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    drawConnectableItem(itemEnterRow,
                        nodeData->propData.funcEnter().isEmpty() ? "(onEnter)" : nodeData->propData.funcEnter(),
                        itemFontSize, nodeData->inputLinkPointColors.first(), true);

    // draw exit row
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
    nodeData->outputLinkPoints.clear();
    nodeData->outputLinkPoints << getConnectPointRect(itemExitRow, false);
    drawConnectableItem(itemExitRow,
                        nodeData->propData.funcExit().isEmpty() ? "(onExit)" : nodeData->propData.funcExit(),
                        itemFontSize, nodeData->outputLinkPointColors.first(), false);

    // draw property rows
    renderPropertyItems(bodyRect, itemExitRow.bottom(), bindStrings);

    GraphicRenderInterface::drawObject();
}
