#include "graphicnode.h"

#include <qmath.h>

GraphicNode::GraphicNode(const QSharedPointer<GraphicNodeData> &data)
    : GraphicObject(data)
    , nodeData(data)
{
    nodeData->propData.nodeId = QUuid::createUuid().toString();
}

bool GraphicNode::selectTest(const QPointF &point) const {
    return nodeData->boundingRect.contains(point);
}

int GraphicNode::testOnLinkPoint(const QPointF &point, bool testInputPoint) const {
    const QList<QRectF>& testTarget = testInputPoint ? nodeData->inputLinkPoints : nodeData->outputLinkPoints;
    for (int i = 0; i < testTarget.size(); i++) {
        auto target = testTarget.at(i);
        auto offset = nodeData->boundingRect.topLeft();
        target.translate(offset);
        target.adjust(-5, -5, 5, 5);
        if (target.contains(point)) {
            return i;
        }
    }
    return -1;
}

void GraphicNode::makeLinkPointActive(int linkIndex, bool isInputPoint) const {
    if (isInputPoint) {
        nodeData->activeInputLinkPointIndex = linkIndex;
    } else {
        nodeData->activeOutputLinkPointIndex = linkIndex;
    }
}

QColor GraphicNode::getLinkPointColor(int linkIndex, bool isInputPoint) const {
    if (isInputPoint) {
        if (linkIndex < nodeData->inputLinkPointColors.size()) {
            return nodeData->inputLinkPointColors.at(linkIndex);
        }
        return nodeData->inputLinkPointColors.last();
    } else {
        if (linkIndex < nodeData->outputLinkPointColors.size()) {
            return nodeData->outputLinkPointColors.at(linkIndex);
        }
        return nodeData->outputLinkPointColors.last();
    }
}

bool GraphicNode::testLinkLineIndexValid(int linkIndex, bool isInputPoint) const {
    return true;
}

ConfigFlowExecutor GraphicNode::toFlowExecutor() const {
    ConfigFlowExecutor executor;
    executor.fromType(objectType());
    executor.text = nodeData->propData.nodeName();
    executor.x = nodeData->renderPosition.x();
    executor.y = nodeData->renderPosition.y();
    executor.enter = nodeData->propData.funcEnter();
    executor.exit = nodeData->propData.funcExit();
    executor.printOnEnter = nodeData->propData.printOnEnter();
    executor.printOnExit = nodeData->propData.printOnExit();
    executor.properties = nodeData->propData.properties();
    executor.uuid = nodeData->propData.nodeId();

    return executor;
}

void GraphicNode::fromExecutor(const ConfigFlowExecutor &executor) const {
    nodeData->propData.nodeName = executor.text();
    nodeData->renderPosition = executor.scenePos();
    nodeData->propData.funcEnter = executor.enter();
    nodeData->propData.funcExit = executor.exit();
    nodeData->propData.printOnEnter = executor.printOnEnter();
    nodeData->propData.printOnExit = executor.printOnExit();
    nodeData->propData.properties = executor.properties();
    if (!executor.uuid().isEmpty()) {
        nodeData->propData.nodeId = executor.uuid();
    }
}

int GraphicNode::nodeNameWidth() const {
    return getTextWidthByFont(nodeData->propData.nodeName(), titleFontSize);
}

int GraphicNode::functionEnterWidth() const {
    return getTextWidthByFont(nodeData->propData.funcEnter(), itemFontSize);
}

int GraphicNode::functionExitWidth() const {
    return getTextWidthByFont(nodeData->propData.funcExit(), itemFontSize);
}

int GraphicNode::minPropertyWidth(QStringList &bindStrings) const {
    int width = 0;
    bindStrings = nodeData->propData.propertyBindDisplayStrings();
    for (const auto& str : bindStrings) {
        width = qMax(width, getTextWidthByFont(str, propertyItemFontSize));
    }
    return width;
}

QRectF GraphicNode::renderNodeBody(int itemRequiredWidth, int itemRequiredHeight) {
    int minTitleWidth = nodeNameWidth() + titlePadding * 2;
    auto bodyRect = getNodeBodyRectFromTopCenter(nodeData->renderPosition,
                                                 qMax(minTitleWidth, itemRequiredWidth),
                                                 itemRequiredHeight + titleHeight + splitHeight);
    nodeData->boundingRect = bodyRect;
    auto guiBodyRect = graphicTransform.toGuiPoint(bodyRect);

    const qreal shadowRadius = 6;
    nodeData->objectRenderCache = QPixmap(qCeil(guiBodyRect.width() + shadowRadius * 2), qCeil(guiBodyRect.height() + shadowRadius * 2));
    nodeData->objectRenderCache.fill(Qt::transparent);
    cachePainter.reset(new QPainter(&nodeData->objectRenderCache));
    cachePainter->setRenderHint(QPainter::Antialiasing);
    cachePainter->translate(shadowRadius, shadowRadius);

    drawNodeBody(guiBodyRect);
    if (nodeData->isRunning) {
        drawNodeRunningState(guiBodyRect, titleHeight);
    }

    bodyRect.moveTopLeft(QPointF(0, 0));
    return bodyRect;
}

void GraphicNode::renderNodeTitle(const QRectF &bodyRect) {
    auto titleDrawRect = QRectF(bodyRect.left(), bodyRect.top(), bodyRect.width(), titleHeight);
    drawNodeTitle(titleDrawRect, nodeData->propData.nodeName(), titleFontSize, titlePadding);
}

void GraphicNode::renderPropertyItems(const QRectF& bodyRect, qreal beginY, const QStringList& bindStrings) {
    if (!bindStrings.isEmpty()) {
        QRectF propertyTitleRect(bodyRect.left(), beginY, bodyRect.width(), propertyTitleHeight);
        drawPropertyTitle(propertyTitleRect, "property set:", propertyTitleFontSize, propertyTitlePadding);
        QRectF propertyItemRect(bodyRect.left(), propertyTitleRect.bottom(), bodyRect.width(), propertyItemHeight);
        for (int i = 0; i < bindStrings.size(); ++i) {
            bool enterSetProp = nodeData->propData.properties()[i].callOnEntered();
            QColor indicatorColor = enterSetProp ? 0x77E000 : 0x00E0E0;
            drawPropertyRow(propertyItemRect.translated(0, i * propertyItemHeight), bindStrings[i], propertyItemFontSize, indicatorColor);
        }
    }
}