#include "nodedelaystate.h"

NodeDelayState::NodeDelayState(const QSharedPointer<NodeDelayStateData> &data, bool initialData)
    : GraphicNode(data)
    , delayStateData(data)
{
    if (initialData) {
        data->propData.nodeName = "延时状态";
        data->inputLinkPointColors << 0x77E000;
        data->outputLinkPointColors << 0x00E0E0;
    }
}

ConfigFlowExecutor NodeDelayState::toFlowExecutor() const {
    auto executor = GraphicNode::toFlowExecutor();
    executor.delay = delayStateData->delayPropData.delayMs();
    executor.delayProperty = delayStateData->delayPropData.delayProperty();

    return executor;
}

void NodeDelayState::fromExecutor(const ConfigFlowExecutor &executor) const {
    GraphicNode::fromExecutor(executor);
    delayStateData->delayPropData.delayMs = executor.delay();
    delayStateData->delayPropData.delayProperty = executor.delayProperty();
}

void NodeDelayState::drawObject() {

    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    minSubItemWidth += itemPadding * 2;

    QString delayProp = delayStateData->delayPropData.delayProperty();
    if (!delayProp.isEmpty()) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(delayProp, itemFontSize));
    }

    // calc min item height
    int minItemHeight = itemHeight * 2 + propertyItemHeight;
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
    delayStateData->inputLinkPoints.clear();
    delayStateData->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    drawConnectableItem(itemEnterRow,
                        delayStateData->propData.funcEnter().isEmpty() ? "(onEnter)" : delayStateData->propData.funcEnter(),
                        itemFontSize, delayStateData->inputLinkPointColors.first(), true);

    // draw exit row
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
    delayStateData->outputLinkPoints.clear();
    delayStateData->outputLinkPoints << getConnectPointRect(itemExitRow, false);
    drawConnectableItem(itemExitRow,
                        delayStateData->propData.funcExit().isEmpty() ? "(onExit)" : delayStateData->propData.funcExit(),
                        itemFontSize, delayStateData->outputLinkPointColors.first(), false);

    // draw delay data row
    QRectF itemDelayDataRow(bodyRect.left(), itemExitRow.bottom(), bodyRect.width(), propertyItemHeight);
    drawIconRow(itemDelayDataRow, ":/res/time.svg", 12,
        !delayProp.isEmpty() ? delayProp : QString("%1 ms").arg(delayStateData->delayPropData.delayMs()), itemFontSize, true);

    // draw property rows
    renderPropertyItems(bodyRect, itemDelayDataRow.bottom(), bindStrings);

    GraphicRenderInterface::drawObject();
}
