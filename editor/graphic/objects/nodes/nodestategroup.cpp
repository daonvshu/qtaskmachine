#include "nodestategroup.h"

NodeStateGroup::NodeStateGroup(const QSharedPointer<NodeStateGroupData> &data, bool initialData)
    : GraphicNode(data)
    , groupData(data)
{
    if (initialData) {
        data->propData.nodeName = "状态组";
        data->inputLinkPointColors << 0x77E000;
        data->outputLinkPointColors << 0xD81E06 << 0xD165A2 << 0x00E0E0;
    }
}

void NodeStateGroup::drawObject() {

    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    //...
    QStringList branchStrings = { "异常状态", "子状态", "下一个状态" };
    //...
    for (const auto &name : branchStrings) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(name, itemFontSize));
    }
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(groupData->groupPropData.errorTriggerFunc(), itemFontSize - 3));
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * (2 + branchStrings.size());
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_State_Group);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    groupData->inputLinkPoints.clear();
    groupData->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    drawConnectableItem(itemEnterRow,
                        groupData->propData.funcEnter().isEmpty() ? "(onEnter)" : groupData->propData.funcEnter(),
                        itemFontSize, groupData->inputLinkPointColors.first(), true);

    // draw exit row
    groupData->outputLinkPoints.clear();
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
    drawPropertyRow(itemExitRow,
                    groupData->propData.funcExit().isEmpty() ? "(onExit)" : groupData->propData.funcExit(),
                    itemFontSize, groupData->outputLinkPointColors.last(), false);

    for (int i = 0; i < branchStrings.size(); ++i) {
        const auto &name = branchStrings.at(i);
        QRectF itemBranchRow = itemExitRow.translated(0, itemHeight * (i + 1));
        groupData->outputLinkPoints << getConnectPointRect(itemBranchRow, false);
        drawDoubleRowConnectableItem(itemBranchRow,
                                     name,
                                     i == 0 ? groupData->groupPropData.errorTriggerFunc() : QString(),
                                     itemFontSize, groupData->outputLinkPointColors.at(i), false);
    }

    // draw property rows
    renderPropertyItems(bodyRect, itemExitRow.bottom() + itemHeight * branchStrings.size(), bindStrings);

    GraphicRenderInterface::drawObject();
}
