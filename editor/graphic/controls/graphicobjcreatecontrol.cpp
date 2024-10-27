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

void GraphicObjCreateControl::addObject(GraphicObjectType type, const QPointF& realPoint) {
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
        editingNodeObject->data->renderPosition = realPoint;
        editingNodeObject->data->selected = true;
        nodeObjects << editingNodeObject;
        d->getControl<GraphicLayerControl>()->updateStaticNodes(nodeObjects, false);
    }
    d->getControl<GraphicLayerControl>()->setActiveNode(editingNodeObject);
    d->view->repaint();
    emit graphicObjectChanged();
}

void GraphicObjCreateControl::copyNodeToMousePoint(const QSharedPointer<GraphicObject> &nodeObject, const QPoint &mousePoint) {
    auto newObj = nodeObject->clone();
    newObj->data->renderPosition = d->getGraphicTransform().toRealPoint(mousePoint);
    if (newObj->data->selected) {
        newObj->data->selected = false;
    }
    nodeObjects << newObj;
    d->getControl<GraphicLayerControl>()->updateStaticNodes(nodeObjects);
    emit graphicObjectChanged();
}

QSharedPointer<GraphicObject> GraphicObjCreateControl::selectTest(const QPoint &mousePoint, bool testSelectedObject) {
    auto realPoint = d->getGraphicTransform().toRealPoint(mousePoint);
    for (const auto& object : nodeObjects) {
        if (object->selectTest(realPoint)) {
            return object;
        }
    }
    for (const auto& linkLine : linkLines) {
        if (linkLine->selectTest(realPoint)) {
            return linkLine;
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

bool GraphicObjCreateControl::testOnSelectedNode(const QPointF &mousePoint) {
    auto realPoint = d->getGraphicTransform().toRealPoint(mousePoint);
    if (editingNodeObject) {
        if (editingNodeObject->selectTest(realPoint)) {
            return true;
        }
    }
    return false;
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

void GraphicObjCreateControl::objTranslateFinished() {
    emit graphicObjectChanged();
}

void GraphicObjCreateControl::removeNodeObject(const QSharedPointer<GraphicObject> &object) {
    nodeObjects.removeOne(object);
    if (editingNodeObject == object) {
        cancelObjActiveSelected();
    }
    d->getControl<GraphicLayerControl>()->updateStaticNodes(nodeObjects);
    for (const auto& linkLine : linkLines) {
        if (linkLine->linkData->linkFromNode == object || linkLine->linkData->linkToNode == object) {
            removeLinkLine(linkLine);
        }
    }
    emit graphicObjectChanged();
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
    cancelSelectedLinkLine();
}

QSharedPointer<GraphicObject> GraphicObjCreateControl::getSelectedNodeObj() {
    return editingNodeObject;
}

void GraphicObjCreateControl::beginActiveLinkLine(const QSharedPointer<GraphicObject> &linkFrom, int linkPointIndex, const QPointF& curMousePoint) {
    editingLinkLine = GraphicLinkLine::create();
    editingLinkLine->linkData->linkFromNode = qSharedPointerCast<GraphicNode>(linkFrom);
    editingLinkLine->linkData->linkFromPointIndex = linkPointIndex;
    editingLinkLine->linkData->tempLinkToPoint = curMousePoint;
    editingLinkLine->linkData->selected = true;
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
        emit graphicObjectChanged();
    }
}

void GraphicObjCreateControl::setLinkLineSelected(const QSharedPointer<GraphicObject> &object) {
    cancelSelectedLinkLine();
    selectedLinkLine = qSharedPointerCast<GraphicLinkLine>(object);
    selectedLinkLine->data->selected = true;
    d->getControl<GraphicLayerControl>()->setActiveLinkLine(selectedLinkLine);
    d->getControl<GraphicLayerControl>()->updateStaticLinkLines(linkLines, false);
}

void GraphicObjCreateControl::cancelSelectedLinkLine() {
    if (selectedLinkLine.isNull()) {
        return;
    }
    selectedLinkLine->data->selected = false;
    d->getControl<GraphicLayerControl>()->cancelActiveLinkLine(selectedLinkLine);
}

void GraphicObjCreateControl::removeLinkLine(const QSharedPointer<GraphicObject> &linkLine) {
    cancelSelectedLinkLine();
    linkLines.removeOne(qSharedPointerCast<GraphicLinkLine>(linkLine));
    d->getControl<GraphicLayerControl>()->updateStaticLinkLines(linkLines);
    emit graphicObjectChanged();
}

QSharedPointer<GraphicLinkLine> GraphicObjCreateControl::getSelectedLinkLine() {
    return selectedLinkLine;
}

bool GraphicObjCreateControl::checkIsAnyLinkLineLinkedToNode(const QSharedPointer<GraphicNode> &node, int linkIndex) {
    for (const auto& linkLine : linkLines) {
        if (linkLine->linkData->linkFromNode == node &&
            linkLine->linkData->linkFromPointIndex == linkIndex)
        {
            return true;
        }
    }
    return false;
}

void GraphicObjCreateControl::clearAll() {
    nodeObjects.clear();
    linkLines.clear();
    editingNodeObject.clear();
    editingLinkLine.clear();
    selectedLinkLine.clear();
    d->getControl<GraphicLayerControl>()->clearAllGraphic();
}
