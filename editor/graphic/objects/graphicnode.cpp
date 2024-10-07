#include "graphicnode.h"

GraphicNode::GraphicNode(const QSharedPointer<GraphicNodeData> &data)
    : GraphicObject(data)
    , nodeData(data)
{

}

bool GraphicNode::selectTest(const QPointF &point) {
    return nodeData->boundingRect.contains(point);
}
