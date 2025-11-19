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
#include "../objects/nodes/nodeloopstate.h"

#include "../objects/actions/nodemoveaction.h"
#include "../objects/actions/objectremoveaction.h"
#include "../objects/actions/objectcopyaction.h"

#include "graphiclayercontrol.h"
#include "appsettings.h"

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
        case GraphicObjectType::Node_Loop_State:
            newObject = NodeLoopState::create();
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
    emit graphicObjectChanged();
}

void GraphicObjCreateControl::copyNodeToMousePoint(const QList<QSharedPointer<GraphicObject>>& nodeObjects, const QPoint &mousePoint) {
    d->graphicObjects.beginMacro("MultiObjectCopy");
    QList<const GraphicObject*> newObjs;
    QHash<const GraphicObject*, const GraphicNode*> srcNodeToCpNode;
    for (const auto& nodeObject : nodeObjects) {
        auto newObj = nodeObject->clone();
        if (newObj->objectType() == GraphicObjectType::Link_Line) {
            auto linkLine = dynamic_cast<const GraphicLinkLine*>(newObj);
            linkLine->linkData->linkFromNode = srcNodeToCpNode[linkLine->linkData->linkFromNode];
            linkLine->linkData->linkToNode = srcNodeToCpNode[linkLine->linkData->linkToNode];
        } else {
            auto offset = nodeObject->data->renderPosition - nodeObjects.first()->data->renderPosition;
            newObj->data->renderPosition = d->getGraphicTransform().toRealPoint(mousePoint) + offset;
            newObj->data->isChanged = true;
            if (newObj->data->selected) {
                newObj->data->selected = false;
            }
            srcNodeToCpNode[nodeObject.data()] = dynamic_cast<const GraphicNode*>(newObj);
        }
        d->graphicObjects.push(newObj);
        newObjs << newObj;
    }

    d->graphicObjects.push(ObjectCopyAction::create(newObjs));
    d->graphicObjects.endMacro();

    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node | GraphicLayerType::Layer_Static_Link);
    emit graphicObjectChanged();
}

const GraphicObject* GraphicObjCreateControl::selectTest(const QPoint &mousePoint, bool testSelectedObject) const {
    auto realPoint = d->getGraphicTransform().toRealPoint(mousePoint);
    auto objects = GraphicObject::getVisibleObjects<GraphicObject>(&d->graphicObjects);
    for (const auto& obj : objects) {
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
    if (!d->multiSelectData.selectedNodes.contains(dynamic_cast<const GraphicNode*>(object))) {
        cancelMultiSelectSelectedState();
        editingNodeObject = object;
        editingNodeObject->data->selected = true;
        editingNodeObject->data->saveRenderPosition();
        d->getControl<GraphicLayerControl>()->setActiveNode(object);
        d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node | GraphicLayerType::Layer_Static_Link);
    } else {
        editingNodeObject = object;
        for (const auto& selectNode : d->multiSelectData.selectedNodes) {
            selectNode->data->saveRenderPosition();
        }
    }
}

void GraphicObjCreateControl::objTranslate(const QPointF& delta) {
    if (editingNodeObject) {
        if (!editingNodeObject->data->tmpMoveSaved) {
            editingNodeObject->data->tmpMoveRenderPoint = editingNodeObject->data->renderPosition;
            editingNodeObject->data->tmpMoveSaved = true;
        }
        auto nodeObj = dynamic_cast<const GraphicNode*>(editingNodeObject);
        if (AppSettings::snapEnabled() && nodeObj != nullptr) {
            snapNodesToOther({ nodeObj }, delta);
        } else {
            editingNodeObject->data->renderPosition = editingNodeObject->data->tmpMoveRenderPoint + delta;
        }
        editingNodeObject->data->isChanged = true;
    }

    for (const auto& selectNode : d->multiSelectData.selectedNodes) {
        if (!selectNode->data->tmpMoveSaved) {
            selectNode->data->tmpMoveRenderPoint = selectNode->data->renderPosition;
            selectNode->data->tmpMoveSaved = true;
        }
    }
    if (AppSettings::snapEnabled()) {
        snapNodesToOther(d->multiSelectData.selectedNodes, delta);
    } else {
        for (const auto& selectNode : d->multiSelectData.selectedNodes) {
            if (selectNode == editingNodeObject) {
                continue;
            }
            selectNode->data->renderPosition = selectNode->data->tmpMoveRenderPoint + delta;
            selectNode->data->isChanged = true;
        }
    }
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Node |
        GraphicLayerType::Layer_Active_Link | GraphicLayerType::Layer_Static_Node);
    emit graphicObjectChanged();
}

void GraphicObjCreateControl::objTranslateFinished() {
    if (editingNodeObject) {
        if (editingNodeObject->data->renderPositionChanged()) {
            d->graphicObjects.push(NodeMoveAction::create(
                d->multiSelectData.selectedNodes + QList { dynamic_cast<const GraphicNode *>(editingNodeObject) })
            );
        } else {
            auto node = dynamic_cast<const GraphicNode*>(editingNodeObject);
            if (d->multiSelectData.selectedNodes.contains(node)) {
                d->multiSelectData.selectedNodes.removeOne(node);
                editingNodeObject = nullptr;
            }
        }
        editingNodeObject->data->tmpMoveSaved = false;
    }
    for (const auto& selectNode : d->multiSelectData.selectedNodes) {
        selectNode->data->tmpMoveSaved = false;
    }
    d->getControl<GraphicLayerControl>()->updateAssistAlignmentLines({});
    emit graphicObjectChanged();
}

void GraphicObjCreateControl::removeNodeObject(const GraphicObject* object) {
    d->graphicObjects.push(ObjectRemoveAction::create(object));
    if (editingNodeObject == object) {
        cancelObjActiveSelected();
    }
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node);
    auto linkLines = GraphicObject::getVisibleObjects<GraphicLinkLine>(&d->graphicObjects);
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

void GraphicObjCreateControl::removeLinkLineOutOfIndex(const GraphicObject* node) {
    cancelSelectedLinkLine();
    bool hasLinkLineRemoved = false;
    auto linkLines = GraphicObject::getVisibleObjects<GraphicLinkLine>(&d->graphicObjects);
    for (const auto& linkLine : linkLines) {
        if (linkLine->linkData->linkFromNode == node) {
            if (auto nodeObj = dynamic_cast<const GraphicNode*>(node)) {
                if (!nodeObj->testLinkLineIndexValid(linkLine->linkData->linkFromPointIndex, false)) {
                    d->graphicObjects.push(ObjectRemoveAction::create(linkLine));
                    hasLinkLineRemoved = true;
                }
            }
        }
    }
    if (!hasLinkLineRemoved) {
        return;
    }
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Link);
    d->graphicObjects.push(new QUndoCommand("CannotUndoOperate"));
}

const GraphicLinkLine* GraphicObjCreateControl::getSelectedLinkLine() const {
    return selectedLinkLine;
}

bool GraphicObjCreateControl::checkIsAnyLinkLineLinkedToNode(const GraphicNode* node, int linkIndex) const {
    auto linkLines = GraphicObject::getVisibleObjects<GraphicLinkLine>(&d->graphicObjects);
    for (const auto& linkLine: linkLines) {
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
    cancelMultiSelectSelectedState();
}

void GraphicObjCreateControl::createMultiSelect(const QRectF& rect) {
    if (!rect.isValid()) {
        return;
    }

    auto realRect = d->getGraphicTransform().toRealPoint(rect);
    QList<const GraphicNode*> selectedNodes;
    auto nodes = GraphicObject::getVisibleObjects<GraphicNode>(&d->graphicObjects);
    for (const auto& node : nodes) {
        if (realRect.intersects(node->nodeData->boundingRect)) {
            selectedNodes << node;
        }
    }
    if (selectedNodes.isEmpty()) {
        return;
    }

    cancelObjActiveSelected();

    d->multiSelectData.selectedNodes = selectedNodes;
    d->getControl<GraphicLayerControl>()->multiSelectObjectsChanged();
}

void GraphicObjCreateControl::cancelMultiSelectSelectedState() const {
    d->multiSelectData.selectedNodes.clear();
    d->getControl<GraphicLayerControl>()->multiSelectObjectsChanged();
}

void GraphicObjCreateControl::removeMultiSelectedObjects() {
    QList<const GraphicObject*> objects;
    auto linkLines = GraphicObject::getVisibleObjects<GraphicLinkLine>(&d->graphicObjects);
    for (const auto& node : d->multiSelectData.selectedNodes) {
        objects << node;
        for (const auto& linkLine : linkLines) {
            if (linkLine->linkData->linkFromNode == node || linkLine->linkData->linkToNode == node) {
                objects << linkLine;
            }
        }
    }
    d->graphicObjects.push(ObjectRemoveAction::create(objects));
    d->multiSelectData.selectedNodes.clear();
    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Link | GraphicLayerType::Layer_Active_Node);
    emit graphicObjectChanged();
}

namespace {

enum Side { S_Left, S_Right, S_CenterX, S_Top, S_Bottom, S_CenterY };

struct SnapCandidate {
    QPointF delta;
    QLineF line;
    qreal length;
};

/**
 * 根据 currentRect 对齐所有 otherRects，返回 X/Y 方向的候选
 */
void collectSnapCandidates(
    const QRectF& currentRect,
    const QList<QRectF>& otherRects,
    int tolerance,
    QList<SnapCandidate>& outX,
    QList<SnapCandidate>& outY)
{
    const std::pair<Side, Side> xRules[] = {
        {S_Left,   S_Left},
        {S_Left,   S_Right},
        {S_Right,  S_Right},
        {S_Right,  S_Left},
        {S_CenterX,S_CenterX}
    };

    const std::pair<Side, Side> yRules[] = {
        {S_Top,    S_Top},
        {S_Top,    S_Bottom},
        {S_Bottom, S_Bottom},
        {S_Bottom, S_Top},
        {S_CenterY,S_CenterY}
    };

    auto getValue = [](const QRectF& r, Side s) {
        switch (s) {
            case S_Left:    return r.left();
            case S_Right:   return r.right();
            case S_CenterX: return r.center().x();
            case S_Top:     return r.top();
            case S_Bottom:  return r.bottom();
            case S_CenterY: return r.center().y();
        }
        return 0.0;
    };

    for (const QRectF& other : otherRects) {

        // ---------- X ----------
        for (auto& rule : xRules) {
            qreal c = getValue(currentRect, rule.first);
            qreal o = getValue(other, rule.second);
            if (qAbs(c - o) > tolerance) continue;

            qreal y1 = qMin(currentRect.top(), other.top());
            qreal y2 = qMax(currentRect.bottom(), other.bottom());

            qreal dx = o - c;

            outX.append({
                QPointF(dx, 0.0),
                QLineF(QPointF(o, y1), QPointF(o, y2)),
                y2 - y1
            });
            break;
        }

        // ---------- Y ----------
        for (auto& rule : yRules) {
            qreal c = getValue(currentRect, rule.first);
            qreal o = getValue(other, rule.second);
            if (qAbs(c - o) > tolerance) continue;

            qreal x1 = qMin(currentRect.left(), other.left());
            qreal x2 = qMax(currentRect.right(), other.right());

            qreal dy = o - c;

            outY.append({
                QPointF(0.0, dy),
                QLineF(QPointF(x1, o), QPointF(x2, o)),
                x2 - x1
            });
            break;
        }
    }
}

QPointF pickBestCandidate(
    const QList<SnapCandidate>& list,
    QLineF* outLine)
{
    if (list.isEmpty()) return QPointF(0, 0);

    const SnapCandidate* best = &list.first();
    for (const SnapCandidate& c : list) {
        if (c.length < best->length)
            best = &c;
    }

    if (outLine) *outLine = best->line;
    return best->delta;
}

} // namespace

void GraphicObjCreateControl::snapNodesToOther(const QList<const GraphicNode*>& nodeObjs, const QPointF& delta) const {
    if (nodeObjs.isEmpty()) return;

    const int tolerance = d->getGraphicTransform().toGuiDx(5);

    // ================================
    // 1. 计算多个节点的合并 currentRect
    // ================================
    QRectF mergedRect;

    for (const GraphicNode* node : nodeObjs) {
        QRectF r(
            node->data->tmpMoveRenderPoint.x() - node->nodeData->boundingRect.width() / 2.f + delta.x(),
            node->data->tmpMoveRenderPoint.y() + delta.y(),
            node->nodeData->boundingRect.width(),
            node->nodeData->boundingRect.height()
        );
        if (mergedRect.isNull())
            mergedRect = r;
        else
            mergedRect = mergedRect.united(r);
    }

    // ===================================================
    // 2. 准备所有 otherRects（同样过滤 nodeObjs 本身）
    // ===================================================
    QList<QRectF> otherRects;
    auto allNodes = GraphicObject::getVisibleObjects<GraphicNode>(&d->graphicObjects);

    for (const auto& other : allNodes) {
        if (nodeObjs.contains(other)) continue;
        otherRects.append(other->nodeData->boundingRect);
    }

    // ==========================
    // 3. 计算吸附候选项
    // ==========================
    QList<SnapCandidate> xcands;
    QList<SnapCandidate> ycands;

    collectSnapCandidates(mergedRect, otherRects, tolerance, xcands, ycands);

    // ==========================
    // 4. 选出最佳 X/Y 候选项
    // ==========================
    QList<QLineF> alignmentLines;
    QPointF finalDelta(0, 0);

    QLineF lineX, lineY;

    QPointF dx = pickBestCandidate(xcands, &lineX);
    QPointF dy = pickBestCandidate(ycands, &lineY);

    finalDelta += dx;
    finalDelta += dy;

    if (dx != QPointF(0,0)) alignmentLines.append(lineX);
    if (dy != QPointF(0,0)) alignmentLines.append(lineY);

    // ==========================
    // 5. 应用到所有节点
    // ==========================
    for (const GraphicNode* node : nodeObjs) {
        node->data->renderPosition = node->data->tmpMoveRenderPoint + delta + finalDelta;
        node->data->isChanged = true;
    }

    d->getControl<GraphicLayerControl>()->updateAssistAlignmentLines(alignmentLines);
}
