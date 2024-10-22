#include "nodeconditionstaterender.h"

NodeConditionStateRender::NodeConditionStateRender(const QSharedPointer<NodeConditionStateData>& data)
    : CommonNodeStateRender(data)
    , d(data)
{
}

void NodeConditionStateRender::drawObject(bool isActiveState) {

    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    QStringList branchIdStrings;
    for (const auto& branchId : d->conditionPropData.branchIds()) {
        auto branchDisplayName = QString("分支（%1）").arg(branchId);
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(branchDisplayName, itemFontSize));
        branchIdStrings << branchDisplayName;
    }
    QString checkFunction = QString("检查函数：") + d->conditionPropData.conditionFunc();
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(checkFunction, itemFontSize));
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * (2 + branchIdStrings.size()) + propertyItemHeight;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight, isActiveState);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Condition_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    if (isActiveState) {
        d->inputLinkPoints.clear();
        d->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    }
    drawConnectableItem(itemEnterRow,
                        d->propData.funcEnter().isEmpty() ? "(onEnter)" : d->propData.funcEnter(),
                        itemFontSize, d->inputLinkPointColors.first(), true,
                        d->activeInputLinkPointIndex != -1);

    // draw exit row
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
    auto outputColor = d->outputLinkPointColors.first();
    drawPropertyRow(itemExitRow,
                        d->propData.funcExit().isEmpty() ? "(onExit)" : d->propData.funcExit(),
                        itemFontSize, outputColor, false);

    // draw branch connect rows
    if (isActiveState) {
        d->outputLinkPoints.clear();
    }
    for (int i = 0; i < branchIdStrings.size(); i++) {
        QRectF itemBranchRow = itemExitRow.translated(0, itemHeight * (i + 1));
        if (isActiveState) {
            d->outputLinkPoints << getConnectPointRect(itemBranchRow, false);
        }
        drawConnectableItem(itemBranchRow,
                            branchIdStrings.at(i),
                            itemFontSize, outputColor, false,
                            d->activeOutputLinkPointIndex == i);
    }

    // draw check function
    QRectF itemCheckRow = itemExitRow.translated(0, itemHeight * (branchIdStrings.size() + 1));
    itemCheckRow.setHeight(propertyItemHeight);
    drawPropertyRow(itemCheckRow, checkFunction, itemFontSize, outputColor, true);

    // draw property rows
    renderPropertyItems(bodyRect, itemCheckRow.bottom(), bindStrings);
}
