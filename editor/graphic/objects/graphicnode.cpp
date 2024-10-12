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
