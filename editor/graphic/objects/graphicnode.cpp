#include "graphicnode.h"

GraphicNode::GraphicNode(const QSharedPointer<GraphicNodeData> &data)
    : GraphicObject(data)
    , nodeData(data)
{

}

bool GraphicNode::selectTest(const QPointF &point) {
    return nodeData->boundingRect.contains(point);
}

int GraphicNode::testOnLinkPoint(const QPointF &point, bool testInputPoint) const {
    const QList<QRectF>& testTarget = testInputPoint ? nodeData->inputLinkPoints : nodeData->outputLinkPoints;
    for (int i = 0; i < testTarget.size(); i++) {
        if (testTarget.at(i).adjusted(-5, -5, 5, 5).contains(point)) {
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

ConfigFlowExecutor GraphicNode::toFlowExecutor() const {
    ConfigFlowExecutor executor;
    executor.fromType(objectType());
    executor.text = nodeData->propData.nodeName();
    executor.x = nodeData->renderPosition.x();
    executor.y = nodeData->renderPosition.y();
    executor.enter = nodeData->propData.funcEnter();
    executor.exit = nodeData->propData.funcExit();
    executor.properties = nodeData->propData.properties();

    return executor;
}

void GraphicNode::fromExecutor(const ConfigFlowExecutor &executor) {
    nodeData->propData.nodeName = executor.text();
    nodeData->renderPosition = executor.scenePos();
    nodeData->propData.funcEnter = executor.enter();
    nodeData->propData.funcExit = executor.exit();
    nodeData->propData.properties = executor.properties();
}
