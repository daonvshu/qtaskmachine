#include "commonnodestaterender.h"

CommonNodeStateRender::CommonNodeStateRender(const QSharedPointer<GraphicObjectData> &data)
    : commonData(qSharedPointerCast<GraphicNodeData>(data))
{
}

int CommonNodeStateRender::nodeNameWidth() const {
    return getTextWidthByFont(commonData->propData.nodeName(), titleFontSize);
}

int CommonNodeStateRender::functionEnterWidth() const {
    return getTextWidthByFont(commonData->propData.funcEnter(), itemFontSize);
}

int CommonNodeStateRender::functionExitWidth() const {
    return getTextWidthByFont(commonData->propData.funcExit(), itemFontSize);
}

int CommonNodeStateRender::minPropertyWidth(QStringList &bindStrings) const {
    int width = 0;
    bindStrings = commonData->propData.propertyBindDisplayStrings();
    for (const auto& str : bindStrings) {
        width = qMax(width, getTextWidthByFont(str, propertyItemFontSize));
    }
    return width;
}

QRectF CommonNodeStateRender::renderNodeBody(int itemRequiredWidth, int itemRequiredHeight, bool isActiveState) {
    int minTitleWidth = nodeNameWidth() + titlePadding * 2;
    auto bodyRect = getNodeBodyRectFromTopCenter(commonData->renderPosition,
                                                 qMax(minTitleWidth, itemRequiredWidth),
                                                 itemRequiredHeight + titleHeight + splitHeight);
    if (isActiveState) {
        commonData->boundingRect = bodyRect;
    }
    drawNodeBody(bodyRect, commonData);
    return bodyRect;
}

void CommonNodeStateRender::renderNodeTitle(const QRectF &bodyRect) {
    auto titleDrawRect = QRectF(bodyRect.left(), bodyRect.top(), bodyRect.width(), titleHeight);
    drawNodeTitle(titleDrawRect, commonData->propData.nodeName(), titleFontSize, titlePadding);
}

void CommonNodeStateRender::renderPropertyItems(const QRectF& bodyRect, qreal beginY, const QStringList& bindStrings) {
    if (!bindStrings.isEmpty()) {
        QRectF propertyTitleRect(bodyRect.left(), beginY, bodyRect.width(), propertyTitleHeight);
        drawPropertyTitle(propertyTitleRect, "property set:", propertyTitleFontSize, propertyTitlePadding);
        QRectF propertyItemRect(bodyRect.left(), propertyTitleRect.bottom(), bodyRect.width(), propertyItemHeight);
        for (int i = 0; i < bindStrings.size(); ++i) {
            bool enterSetProp = commonData->propData.properties()[i].callOnEntered();
            QColor indicatorColor = enterSetProp ? 0x77E000 : 0x00E0E0;
            drawPropertyRow(propertyItemRect.translated(0, i * propertyItemHeight), bindStrings[i], propertyItemFontSize, indicatorColor);
        }
    }
}

