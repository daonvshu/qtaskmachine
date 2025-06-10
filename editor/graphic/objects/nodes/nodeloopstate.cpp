#include "nodeloopstate.h"

NodeLoopState::NodeLoopState(const QSharedPointer<NodeLoopStateData>& data, bool initialData)
    : GraphicNode(data)
    , loopStateData(data)
{
    if (initialData) {
        data->propData.nodeName = "循环执行状态";
        data->inputLinkPointColors << 0x77E000;
        data->outputLinkPointColors << 0xD1682F << 0x00E0E0;
    }
}

bool NodeLoopState::testLinkLineIndexValid(int linkIndex, bool isInputPoint) const {
    if (isInputPoint) {
        return linkIndex <= 0;
    }
    return linkIndex < 2;
}

ConfigFlowExecutor NodeLoopState::toFlowExecutor() const {
    auto executor = GraphicNode::toFlowExecutor();
    executor.condition = loopStateData->loopStatePropData.exitFunc();
    executor.total = loopStateData->loopStatePropData.total();

    return executor;
}

void NodeLoopState::fromExecutor(const ConfigFlowExecutor& executor) const {
    GraphicNode::fromExecutor(executor);
    loopStateData->loopStatePropData.exitFunc = executor.condition();
    loopStateData->loopStatePropData.total = executor.total();
}

void NodeLoopState::drawObject() {
    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    //...
    QStringList branchStrings = { "下一次循环", "循环退出" };
    //...
    for (const auto &name : branchStrings) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(name, itemFontSize));
    }

    auto totalCheckProp = loopStateData->loopStatePropData.total();
    if (totalCheckProp.isEmpty() || totalCheckProp.toInt() == -1) {
        totalCheckProp = "（无限制）";
    }
    totalCheckProp.prepend("循环次数：");
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(totalCheckProp, itemFontSize));

    auto exitCheckFunction = loopStateData->loopStatePropData.exitFunc();
    exitCheckFunction.prepend("退出检查函数：");
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(exitCheckFunction, itemFontSize));

    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * (2 + branchStrings.size()) + 2 * propertyItemHeight;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Loop_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    loopStateData->inputLinkPoints.clear();
    loopStateData->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    drawConnectableItem(itemEnterRow,
                        loopStateData->propData.funcEnter().isEmpty() ? "(onEnter)" : loopStateData->propData.funcEnter(),
                        itemFontSize, loopStateData->inputLinkPointColors.first(), true);

    // draw exit row
    loopStateData->outputLinkPoints.clear();
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
    drawPropertyRow(itemExitRow,
                    loopStateData->propData.funcExit().isEmpty() ? "(onExit)" : loopStateData->propData.funcExit(),
                    itemFontSize, loopStateData->outputLinkPointColors.last(), false);

    for (int i = 0; i < branchStrings.size(); ++i) {
        const auto &name = branchStrings.at(i);
        QRectF itemBranchRow = itemExitRow.translated(0, itemHeight * (i + 1));
        loopStateData->outputLinkPoints << getConnectPointRect(itemBranchRow, false);
        drawDoubleRowConnectableItem(itemBranchRow,
                                     name,
                                     QString(),
                                     itemFontSize, loopStateData->outputLinkPointColors.at(i), false);
    }

    // draw total row
    QRectF itemTotalRow = itemExitRow.translated(0, itemHeight * (branchStrings.size() + 1));
    itemTotalRow.setHeight(propertyItemHeight);
    drawPropertyRow(itemTotalRow, totalCheckProp, itemFontSize, loopStateData->outputLinkPointColors.first(), true);

    // draw exit function
    QRectF itemExitFunctionRow = itemTotalRow.translated(0, propertyItemHeight);
    itemExitFunctionRow.setHeight(propertyItemHeight);
    drawPropertyRow(itemExitFunctionRow, exitCheckFunction, itemFontSize, loopStateData->outputLinkPointColors.first(), true);

    // draw property rows
    renderPropertyItems(bodyRect, itemExitRow.bottom() + itemHeight * branchStrings.size(), bindStrings);

    GraphicRenderInterface::drawObject();
}

bool NodeLoopState::containKeywords(const QString& keywords) const {
    if (loopStateData->loopStatePropData.searchContent().contains(keywords)) {
        return true;
    }
    return GraphicNode::containKeywords(keywords);
}
