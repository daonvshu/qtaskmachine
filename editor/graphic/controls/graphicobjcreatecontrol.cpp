#include "graphicobjcreatecontrol.h"

#include "../objects/nodes/nodenormalstate.h"

#include "graphiclayercontrol.h"

GraphicObjCreateControl::GraphicObjCreateControl(const QSharedPointer<GraphicControlSharedData> &data, QObject *parent)
    : GraphicControl(data, parent)
{
}

void GraphicObjCreateControl::addObject(GraphicObjectType type, const QPoint& mousePoint) {
    cancelObjActiveSelected();
    switch (type) {
        case GraphicObjectType::Node_Begin_State:
            break;
        case GraphicObjectType::Node_End_State:
            break;
        case GraphicObjectType::Node_Normal_State:
            activeObject = NodeNormalState::create();
            break;
        case GraphicObjectType::Node_Delay_State:
            break;
        case GraphicObjectType::Node_Event_State:
            break;
        case GraphicObjectType::Node_MultiEvent_State:
            break;
        case GraphicObjectType::Node_Condition_State:
            break;
        case GraphicObjectType::Node_State_Group:
            break;
        case GraphicObjectType::Node_Recovery_State:
            break;
        default:
            break;
    }
    if (activeObject) {
        activeObject->data->renderPosition = d->getGraphicTransform().toRealPoint(mousePoint);
        activeObject->data->selected = true;
        graphicObjects << activeObject;
        d->getControl<GraphicLayerControl>()->updateStaticNodes(graphicObjects);
    }
    d->getControl<GraphicLayerControl>()->setActiveNode(activeObject);
    d->view->repaint();
}

QSharedPointer<GraphicObject> GraphicObjCreateControl::selectTest(const QPoint &mousePoint) {
    auto realPoint = d->getGraphicTransform().toRealPoint(mousePoint);
    for (const auto& object : graphicObjects) {
        if (object->selectTest(realPoint)) {
            return object;
        }
    }
    return nullptr;
}

void GraphicObjCreateControl::setObjectSelected(const QSharedPointer<GraphicObject> &object) {
    cancelObjActiveSelected();
    activeObject = object;
    activeObject->data->selected = true;
    d->getControl<GraphicLayerControl>()->setActiveNode(object);
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node);
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Link);
}

void GraphicObjCreateControl::objTranslate(const QPointF& delta) {
    if (activeObject) {
        activeObject->data->renderPosition += delta;
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Node);
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Link);
    }
}

void GraphicObjCreateControl::cancelObjActiveSelected() {
    if (activeObject) {
        activeObject->data->selected = false;
        d->getControl<GraphicLayerControl>()->setActiveNode(nullptr);
        if (activeLinkLine.isNull()) {
            d->getControl<GraphicLayerControl>()->cancelAllActiveLinkLine();
        }
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node);
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Link);
    }
}

void GraphicObjCreateControl::beginActiveLinkLine(const QSharedPointer<GraphicObject> &linkFrom, int linkPointIndex, const QPointF& curMousePoint) {
    activeLinkLine = GraphicLinkLine::create();
    activeLinkLine->linkData->linkFromNodeData = qSharedPointerCast<GraphicNode>(linkFrom)->data;
    activeLinkLine->linkData->linkFromPointIndex = linkPointIndex;
    activeLinkLine->linkData->tempLinkToPoint = curMousePoint;
    linkLines << activeLinkLine;
    d->getControl<GraphicLayerControl>()->setActiveLinkLine(activeLinkLine);
    d->getControl<GraphicLayerControl>()->updateStaticLinkLines(linkLines);
}

void GraphicObjCreateControl::updateActiveLinkLineToPoint(const QSharedPointer<GraphicObject>& linkTo, int linkPointIndex, const QPointF &curMousePoint) {
    if (activeLinkLine) {
        activeLinkLine->linkData->linkToNodeData = linkTo.isNull() ? nullptr : qSharedPointerCast<GraphicNode>(linkTo)->data;
        activeLinkLine->linkData->linkToPointIndex = linkPointIndex;
        activeLinkLine->linkData->tempLinkToPoint = curMousePoint;
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Link);
    }
}

void GraphicObjCreateControl::releaseActiveLinkLine() {
    if (activeLinkLine) {
        activeLinkLine->linkData->isEditing = false;
        if (activeLinkLine->linkData->linkToNodeData.isNull()) {
            linkLines.removeOne(activeLinkLine);
            d->getControl<GraphicLayerControl>()->updateStaticLinkLines(linkLines);
            activeLinkLine = nullptr;
        } else {
            d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Link);
        }
        if (activeLinkLine) {
            bool linkToSelectedNode = activeLinkLine->linkData->linkFromNodeData == activeObject->data ||
                    activeLinkLine->linkData->linkToNodeData == activeObject->data;
            if (linkToSelectedNode) {
                activeLinkLine->linkData->selected = true;
            } else {
                d->getControl<GraphicLayerControl>()->cancelActiveLinkLine(activeLinkLine);
            }
            activeLinkLine = nullptr;
        }
    }
}
