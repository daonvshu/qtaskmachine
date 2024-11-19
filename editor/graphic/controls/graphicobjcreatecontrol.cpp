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

#include "../objects/actions/nodemoveaction.h"
#include "../objects/actions/objectremoveaction.h"

#include "graphiclayercontrol.h"

GraphicObjCreateControl::GraphicObjCreateControl(const QSharedPointer<GraphicControlSharedData> &data, QObject *parent)
    : GraphicControl(data, parent)
{
}

void GraphicObjCreateControl::addObject(GraphicObjectType type, const QPointF& realPoint) {
    cancelObjActiveSelected();
    GraphicNode* newObject = nullptr;
    switch (type) {
        case GraphicObjectType::Node_Begin_State:
            newObject = NodeBeginState::create();
            break;
        case GraphicObjectType::Node_End_State:
            newObject = NodeEndState::create();
            break;
        case GraphicObjectType::Node_Normal_State:
            newObject = NodeNormalState::create();
            break;
        case GraphicObjectType::Node_Delay_State:
            newObject = NodeDelayState::create();
            break;
        case GraphicObjectType::Node_Event_State:
            newObject = NodeEventState::create();
            break;
        case GraphicObjectType::Node_MultiEvent_State:
            newObject = NodeMultiEventState::create();
            break;
        case GraphicObjectType::Node_Condition_State:
            newObject = NodeConditionState::create();
            break;
        case GraphicObjectType::Node_State_Group:
            newObject = NodeStateGroup::create();
            break;
        case GraphicObjectType::Node_Recovery_State:
            newObject = NodeRecoveryState::create();
            break;
        default:
            break;
    }
    if (newObject) {
        editingNodeObject = newObject;
        editingNodeObject->data->renderPosition = realPoint;
        editingNodeObject->data->selected = true;
        d->graphicObjects.push(newObject);
    }
    d->getControl<GraphicLayerControl>()->setActiveNode(newObject);
    d->view->repaint();
    emit graphicObjectChanged();
}

void GraphicObjCreateControl::copyNodeToMousePoint(const GraphicObject* nodeObject, const QPoint &mousePoint) {
    auto newObj = nodeObject->clone();
    newObj->data->renderPosition = d->getGraphicTransform().toRealPoint(mousePoint);
    newObj->data->isChanged = true;
    if (newObj->data->selected) {
        newObj->data->selected = false;
    }
    d->graphicObjects.push(newObj);
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node);
    emit graphicObjectChanged();
}

const GraphicObject* GraphicObjCreateControl::selectTest(const QPoint &mousePoint, bool testSelectedObject) const {
    auto realPoint = d->getGraphicTransform().toRealPoint(mousePoint);
    for (int i = 0; i < d->graphicObjects.index(); i++) {
        auto obj = dynamic_cast<const GraphicObject*>(d->graphicObjects.command(i));
        if (obj == nullptr) {
            continue;
        }
        if (obj->data->assignRemoved) {
            continue;
        }
        if (obj->selectTest(realPoint)) {
            return obj;
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

bool GraphicObjCreateControl::testOnSelectedNode(const QPointF &mousePoint) const {
    auto realPoint = d->getGraphicTransform().toRealPoint(mousePoint);
    if (editingNodeObject) {
        if (editingNodeObject->selectTest(realPoint)) {
            return true;
        }
    }
    return false;
}

void GraphicObjCreateControl::setObjectSelected(const GraphicObject* object) {
    cancelObjActiveSelected();
    editingNodeObject = object;
    editingNodeObject->data->selected = true;
    editingNodeObject->data->saveRenderPosition();
    d->getControl<GraphicLayerControl>()->setActiveNode(object);
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node | GraphicLayerType::Layer_Static_Link);
}

void GraphicObjCreateControl::objTranslate(const QPointF& delta) const {
    if (editingNodeObject) {
        editingNodeObject->data->renderPosition += delta;
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Node | GraphicLayerType::Layer_Active_Link);
    }
}

void GraphicObjCreateControl::objTranslateFinished() {
    if (editingNodeObject->data->renderPositionChanged()) {
        d->graphicObjects.push(NodeMoveAction::create(dynamic_cast<const GraphicNode *>(editingNodeObject)));
    }
    emit graphicObjectChanged();
}

void GraphicObjCreateControl::removeNodeObject(const GraphicObject* object) {
    d->graphicObjects.push(ObjectRemoveAction::create(object));
    if (editingNodeObject == object) {
        cancelObjActiveSelected();
    }
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node);
    for (int i = 0; i < d->graphicObjects.index(); i++) {
        auto linkLine = dynamic_cast<const GraphicLinkLine*>(d->graphicObjects.command(i));
        if (linkLine == nullptr) {
            continue;
        }
        if (linkLine->data->assignRemoved) {
            continue;
        }
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
        Q_ASSERT(editingLinkLine == nullptr);
        d->getControl<GraphicLayerControl>()->cancelAllActiveLinkLine();
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node | GraphicLayerType::Layer_Static_Link);
    }
    cancelSelectedLinkLine();
}

const GraphicObject* GraphicObjCreateControl::getSelectedNodeObj() const {
    return editingNodeObject;
}

void GraphicObjCreateControl::beginActiveLinkLine(const GraphicObject* linkFrom, int linkPointIndex, const QPointF& curMousePoint) {
    auto newLinkLine = GraphicLinkLine::create();
    editingLinkLine = newLinkLine;
    editingLinkLine->linkData->linkFromNode = dynamic_cast<const GraphicNode*>(linkFrom);
    editingLinkLine->linkData->linkFromPointIndex = linkPointIndex;
    editingLinkLine->linkData->tempLinkToPoint = curMousePoint;
    editingLinkLine->linkData->selected = true;
    d->graphicObjects.push(newLinkLine);
    d->getControl<GraphicLayerControl>()->setActiveLinkLine(newLinkLine);
}

void GraphicObjCreateControl::updateActiveLinkLineToPoint(const GraphicObject* linkTo, int linkPointIndex, const QPointF &curMousePoint) const {
    if (editingLinkLine) {
        editingLinkLine->linkData->linkToNode = dynamic_cast<const GraphicNode*>(linkTo);
        editingLinkLine->linkData->linkToPointIndex = linkPointIndex;
        editingLinkLine->linkData->tempLinkToPoint = curMousePoint;
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Link);
    }
}

void GraphicObjCreateControl::releaseActiveLinkLine() {
    if (editingLinkLine) {
        editingLinkLine->linkData->isEditing = false;
        if (editingLinkLine->linkData->linkToNode == nullptr) {
            d->graphicObjects.undo(); //undo to assign link line is removed
            d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Link);
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

void GraphicObjCreateControl::setLinkLineSelected(const GraphicObject* object) {
    cancelSelectedLinkLine();
    selectedLinkLine = dynamic_cast<const GraphicLinkLine*>(object);
    selectedLinkLine->data->selected = true;
    d->getControl<GraphicLayerControl>()->setActiveLinkLine(const_cast<GraphicLinkLine*>(selectedLinkLine));
}

void GraphicObjCreateControl::cancelSelectedLinkLine() {
    if (selectedLinkLine == nullptr) {
        return;
    }
    selectedLinkLine->data->selected = false;
    d->getControl<GraphicLayerControl>()->cancelActiveLinkLine(selectedLinkLine);
    selectedLinkLine = nullptr;
}

void GraphicObjCreateControl::removeLinkLine(const GraphicObject* linkLine) {
    cancelSelectedLinkLine();
    d->graphicObjects.push(ObjectRemoveAction::create(linkLine));
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Link);
    emit graphicObjectChanged();
}

const GraphicLinkLine* GraphicObjCreateControl::getSelectedLinkLine() const {
    return selectedLinkLine;
}

bool GraphicObjCreateControl::checkIsAnyLinkLineLinkedToNode(const GraphicNode* node, int linkIndex) const {
    for (int i = 0; i < d->graphicObjects.index(); i++) {
        auto obj = dynamic_cast<const GraphicObject*>(d->graphicObjects.command(i));
        if (obj == nullptr) {
            continue;
        }
        if (obj->data->assignRemoved) {
            continue;
        }
        auto linkLine = dynamic_cast<const GraphicLinkLine*>(obj);
        if (linkLine == nullptr) {
            continue;
        }
        if (linkLine->linkData->linkFromNode == node &&
            linkLine->linkData->linkFromPointIndex == linkIndex)
        {
            return true;
        }
    }
    return false;
}

void GraphicObjCreateControl::clearAll() {
    d->graphicObjects.clear();
    editingNodeObject = nullptr;
    editingLinkLine = nullptr;
    selectedLinkLine = nullptr;
    d->getControl<GraphicLayerControl>()->clearAllGraphic();
}
