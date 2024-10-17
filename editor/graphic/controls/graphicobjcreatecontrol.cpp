#include "graphicobjcreatecontrol.h"

#include "../objects/nodes/nodenormalstate.h"
#include "../objects/nodes/nodebeginstate.h"
#include "../objects/nodes/nodeendstate.h"
#include "../objects/nodes/nodedelaystate.h"
#include "../objects/nodes/nodeconditionstate.h"
#include "../objects/nodes/nodeeventstate.h"
#include "../objects/nodes/nodemultieventstate.h"
#include "../objects/nodes/nodestategroup.h"
#include "../objects/nodes/noderecoverystate.h"

#include "graphiclayercontrol.h"

GraphicObjCreateControl::GraphicObjCreateControl(const QSharedPointer<GraphicControlSharedData> &data, QObject *parent)
    : GraphicControl(data, parent)
{
}

void GraphicObjCreateControl::addObject(GraphicObjectType type, const QPoint& mousePoint) {
    cancelObjActiveSelected();
    switch (type) {
        case GraphicObjectType::Node_Begin_State:
            editingNodeObject = NodeBeginState::create();
            break;
        case GraphicObjectType::Node_End_State:
            editingNodeObject = NodeEndState::create();
            break;
        case GraphicObjectType::Node_Normal_State:
            editingNodeObject = NodeNormalState::create();
            break;
        case GraphicObjectType::Node_Delay_State:
            editingNodeObject = NodeDelayState::create();
            break;
        case GraphicObjectType::Node_Event_State:
            editingNodeObject = NodeEventState::create();
            break;
        case GraphicObjectType::Node_MultiEvent_State:
            editingNodeObject = NodeMultiEventState::create();
            break;
        case GraphicObjectType::Node_Condition_State:
            editingNodeObject = NodeConditionState::create();
            break;
        case GraphicObjectType::Node_State_Group:
            editingNodeObject = NodeStateGroup::create();
            break;
        case GraphicObjectType::Node_Recovery_State:
            editingNodeObject = NodeRecoveryState::create();
            break;
        default:
            break;
    }
    if (editingNodeObject) {
        editingNodeObject->data->renderPosition = d->getGraphicTransform().toRealPoint(mousePoint);
        editingNodeObject->data->selected = true;
        nodeObjects << editingNodeObject;
        d->getControl<GraphicLayerControl>()->updateStaticNodes(nodeObjects, false);
    }
    d->getControl<GraphicLayerControl>()->setActiveNode(editingNodeObject);
    d->view->repaint();
}

QSharedPointer<GraphicObject> GraphicObjCreateControl::selectTest(const QPoint &mousePoint, bool testSelectedObject) {
    auto realPoint = d->getGraphicTransform().toRealPoint(mousePoint);
    for (const auto& object : nodeObjects) {
        if (object->selectTest(realPoint)) {
            return object;
        }
    }
    if (testSelectedObject) {
        if (editingNodeObject) {
            if (editingNodeObject->selectTest(realPoint)) {
                return editingNodeObject;
            }
        }
    }
    return nullptr;
}

void GraphicObjCreateControl::setObjectSelected(const QSharedPointer<GraphicObject> &object) {
    cancelObjActiveSelected();
    editingNodeObject = object;
    editingNodeObject->data->selected = true;
    d->getControl<GraphicLayerControl>()->setActiveNode(object);
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node | GraphicLayerType::Layer_Static_Link);
}

void GraphicObjCreateControl::objTranslate(const QPointF& delta) {
    if (editingNodeObject) {
        editingNodeObject->data->renderPosition += delta;
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Node | GraphicLayerType::Layer_Active_Link);
    }
}

void GraphicObjCreateControl::cancelObjActiveSelected() {
    if (editingNodeObject) {
        editingNodeObject->data->selected = false;
        editingNodeObject = nullptr;
        d->getControl<GraphicLayerControl>()->setActiveNode(nullptr);
        Q_ASSERT(editingLinkLine.isNull());
        d->getControl<GraphicLayerControl>()->cancelAllActiveLinkLine();
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node | GraphicLayerType::Layer_Static_Link);
    }
}

void GraphicObjCreateControl::beginActiveLinkLine(const QSharedPointer<GraphicObject> &linkFrom, int linkPointIndex, const QPointF& curMousePoint) {
    editingLinkLine = GraphicLinkLine::create();
    editingLinkLine->linkData->linkFromNode = qSharedPointerCast<GraphicNode>(linkFrom);
    editingLinkLine->linkData->linkFromPointIndex = linkPointIndex;
    editingLinkLine->linkData->tempLinkToPoint = curMousePoint;
    linkLines << editingLinkLine;
    d->getControl<GraphicLayerControl>()->setActiveLinkLine(editingLinkLine);
    d->getControl<GraphicLayerControl>()->updateStaticLinkLines(linkLines, false);
}

void GraphicObjCreateControl::updateActiveLinkLineToPoint(const QSharedPointer<GraphicObject>& linkTo, int linkPointIndex, const QPointF &curMousePoint) {
    if (editingLinkLine) {
        editingLinkLine->linkData->linkToNode = qSharedPointerCast<GraphicNode>(linkTo);
        editingLinkLine->linkData->linkToPointIndex = linkPointIndex;
        editingLinkLine->linkData->tempLinkToPoint = curMousePoint;
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Link);
    }
}

void GraphicObjCreateControl::releaseActiveLinkLine() {
    if (editingLinkLine) {
        editingLinkLine->linkData->isEditing = false;
        if (editingLinkLine->linkData->linkToNode.isNull()) {
            linkLines.removeOne(editingLinkLine);
            d->getControl<GraphicLayerControl>()->updateStaticLinkLines(linkLines);
            d->getControl<GraphicLayerControl>()->cancelActiveLinkLine(editingLinkLine);
            editingLinkLine = nullptr;
        } else {
            d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Link);
        }
        if (editingLinkLine) {
            bool linkToSelectedNode = editingLinkLine->linkData->linkFromNode == editingNodeObject ||
                                      editingLinkLine->linkData->linkToNode == editingNodeObject;
            if (linkToSelectedNode) {
                editingLinkLine->linkData->selected = true;
                d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Link);
            } else {
                d->getControl<GraphicLayerControl>()->cancelActiveLinkLine(editingLinkLine);
            }
            editingLinkLine = nullptr;
        }
    }
}
