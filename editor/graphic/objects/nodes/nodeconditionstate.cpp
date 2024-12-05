#include "nodeconditionstate.h"

NodeConditionState::NodeConditionState(const QSharedPointer<NodeConditionStateData>& data, bool initialData)
    : GraphicNode(data)
    , conditionStateData(data)
{
    if (initialData) {
        data->propData.nodeName = "条件分支状态";
        data->inputLinkPointColors << 0x77E000;
        data->outputLinkPointColors << 0x00E0E0;
    }
}

ConfigFlowExecutor NodeConditionState::toFlowExecutor() const {
    auto executor = GraphicNode::toFlowExecutor();
    executor.condition = conditionStateData->conditionPropData.conditionFunc();

    return executor;
}

void NodeConditionState::fromExecutor(const ConfigFlowExecutor &executor) const {
    GraphicNode::fromExecutor(executor);
    conditionStateData->conditionPropData.conditionFunc = executor.condition();
    conditionStateData->conditionPropData.branchIds().clear();
    conditionStateData->conditionPropData.branchNames().clear();
}

void NodeConditionState::drawObject() {
    
    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    QStringList branchIdStrings;
    const auto& ids = conditionStateData->conditionPropData.branchIds();
    const auto& names = conditionStateData->conditionPropData.branchNames();
    for (int i = 0; i < qMin(ids.size(), names.size()); ++i) {
        auto branchDisplayName = QString("%1（%2）").arg(names[i].isEmpty() ? "[分支]" : names[i]).arg(ids[i]);
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(branchDisplayName, itemFontSize));
        branchIdStrings << branchDisplayName;
    }
    QString checkFunction = QString("检查函数：") + conditionStateData->conditionPropData.conditionFunc();
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(checkFunction, itemFontSize));
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * (2 + branchIdStrings.size()) + propertyItemHeight;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Condition_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    conditionStateData->inputLinkPoints.clear();
    conditionStateData->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    drawConnectableItem(itemEnterRow,
                        conditionStateData->propData.funcEnter().isEmpty() ? "(onEnter)" : conditionStateData->propData.funcEnter(),
                        itemFontSize, conditionStateData->inputLinkPointColors.first(), true);

    // draw exit row
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
    auto outputColor = conditionStateData->outputLinkPointColors.first();
    drawPropertyRow(itemExitRow,
                    conditionStateData->propData.funcExit().isEmpty() ? "(onExit)" : conditionStateData->propData.funcExit(),
                    itemFontSize, outputColor, false);

    // draw branch connect rows
    conditionStateData->outputLinkPoints.clear();
    for (int i = 0; i < branchIdStrings.size(); i++) {
        QRectF itemBranchRow = itemExitRow.translated(0, itemHeight * (i + 1));
        conditionStateData->outputLinkPoints << getConnectPointRect(itemBranchRow, false);
        drawConnectableItem(itemBranchRow,
                            branchIdStrings.at(i),
                            itemFontSize, outputColor, false);
    }

    // draw check function
    QRectF itemCheckRow = itemExitRow.translated(0, itemHeight * (branchIdStrings.size() + 1));
    itemCheckRow.setHeight(propertyItemHeight);
    drawPropertyRow(itemCheckRow, checkFunction, itemFontSize, outputColor, true);

    // draw property rows
    renderPropertyItems(bodyRect, itemCheckRow.bottom(), bindStrings);

    GraphicRenderInterface::drawObject();
}
