#include "nodemultieventstate.h"

NodeMultiEventState::NodeMultiEventState(const QSharedPointer<NodeMultiEventStateData> &data, bool initialData)
    : GraphicNode(data)
    , eventStateData(data)
{
    if (initialData) {
        data->propData.nodeName = "复合事件状态";
        data->inputLinkPointColors << 0x77E000;
        data->outputLinkPointColors << 0x00E0E0;
    }
}

void NodeMultiEventState::fromExecutor(const ConfigFlowExecutor &executor) const {
    GraphicNode::fromExecutor(executor);
    eventStateData->eventPropData.events().clear();
}

void NodeMultiEventState::drawObject() {

    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    auto eventRows = eventStateData->eventPropData.events();
    for (auto & eventRow : eventRows) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(eventRow.triggerFunc() + QString("[%1]").arg(eventRow.branchId()), itemFontSize));
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(eventRow.checkFunc(), itemFontSize - 3));
    }
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * (2 + eventRows.size());
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Event_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    eventStateData->inputLinkPoints.clear();
    eventStateData->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    drawConnectableItem(itemEnterRow,
                        eventStateData->propData.funcEnter().isEmpty() ? "(onEnter)" : eventStateData->propData.funcEnter(),
                        itemFontSize, eventStateData->inputLinkPointColors.first(), true);

    // draw exit row
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
    auto outputColor = eventStateData->outputLinkPointColors.first();
    drawPropertyRow(itemExitRow,
                    eventStateData->propData.funcExit().isEmpty() ? "(onExit)" : eventStateData->propData.funcExit(),
                    itemFontSize, outputColor, false);

    // draw normal trigger event row
    eventStateData->outputLinkPoints.clear();
    for (int i = 0; i < eventRows.size(); ++i) {
        QRectF itemEventRow = itemExitRow.translated(0, itemHeight * (i + 1));
        eventStateData->outputLinkPoints << getConnectPointRect(itemEventRow, false);
        QString mainEventText = QString("%0[%1]").arg(eventRows[i].triggerFunc()).arg(eventRows[i].branchId());
        drawDoubleRowConnectableItem(itemEventRow,
                                     mainEventText,
                                     eventRows[i].checkFunc(),
                                     itemFontSize, outputColor, false);
    }

    // draw property rows
    renderPropertyItems(bodyRect, itemExitRow.bottom() + itemHeight * eventRows.size(), bindStrings);

    GraphicRenderInterface::drawObject();
}
