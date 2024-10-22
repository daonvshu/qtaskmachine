#include "nodegroupstaterender.h"

NodeGroupStateRender::NodeGroupStateRender(const QSharedPointer<NodeStateGroupData> &data)
    : CommonNodeStateRender(data)
    , d(data)
{
}

void NodeGroupStateRender::drawObject(bool isActiveState) {

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
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(d->groupPropData.errorTriggerFunc(), itemFontSize - 3));
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * (2 + branchStrings.size());
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight, isActiveState);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_State_Group);

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
    if (isActiveState) {
        d->outputLinkPoints.clear();
    }
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
    drawPropertyRow(itemExitRow,
                    d->propData.funcExit().isEmpty() ? "(onExit)" : d->propData.funcExit(),
                    itemFontSize, d->outputLinkPointColors.last(), false);

    for (int i = 0; i < branchStrings.size(); ++i) {
        const auto &name = branchStrings.at(i);
        QRectF itemBranchRow = itemExitRow.translated(0, itemHeight * (i + 1));
        if (isActiveState) {
            d->outputLinkPoints << getConnectPointRect(itemBranchRow, false);
        }
        drawDoubleRowConnectableItem(itemBranchRow,
                                     name,
                                     i == 0 ? d->groupPropData.errorTriggerFunc() : QString(),
                                     itemFontSize, d->outputLinkPointColors.at(i), false,
                                     d->activeOutputLinkPointIndex == i);
    }

    // draw property rows
    renderPropertyItems(bodyRect, itemExitRow.bottom() + itemHeight * branchStrings.size(), bindStrings);
}
