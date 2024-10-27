#include "graphicview.h"

#include "graphiccontrol.h"

#include "controls/graphiclayercontrol.h"
#include "controls/mouseactioncontrol.h"
#include "controls/transformcontrol.h"
#include "controls/graphicobjcreatecontrol.h"

#include "objects/nodes/nodeeventstate.h"
#include "objects/nodes/nodemultieventstate.h"
#include "objects/nodes/nodeconditionstate.h"
#include "objects/nodes/nodestategroup.h"

#include "data/configflows.h"

#include <qpainter.h>

GraphicView::GraphicView(QWidget *parent)
    : QWidget(parent)
    , backgroundColor(0x35363C)
{
    controls = new GraphicControlWrapper(this, this);
    controls->add<GraphicLayerControl>();
    controls->add<MouseActionControl>();
    controls->add<TransformControl>();
    controls->add<GraphicObjCreateControl>();

    controls->get<MouseActionControl>()->installShortcut();
    connect(controls->get<GraphicObjCreateControl>(), &GraphicObjCreateControl::graphicObjectChanged, this, &GraphicView::saveFlow);

    setMouseTracking(true);
}

void GraphicView::resizeEvent(QResizeEvent *event) {

    controls->get<TransformControl>()->coordinateReload();
    controls->get<GraphicLayerControl>()->graphLayerReload();

    QWidget::resizeEvent(event);
}

void GraphicView::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), backgroundColor);

    controls->get<TransformControl>()->scaleByDefault();
    controls->get<GraphicLayerControl>()->graphLayerRepaint(&painter);
}

void GraphicView::mousePressEvent(QMouseEvent *event) {
    controls->get<MouseActionControl>()->mousePress(event);
}

void GraphicView::mouseReleaseEvent(QMouseEvent *event) {
    controls->get<MouseActionControl>()->mouseRelease(event);
}

void GraphicView::mouseMoveEvent(QMouseEvent *event) {
    controls->get<MouseActionControl>()->mouseMove(event);
}

void GraphicView::wheelEvent(QWheelEvent *event) {
    controls->get<MouseActionControl>()->wheelEvent(event);
}

void GraphicView::contextMenuEvent(QContextMenuEvent *event) {
    controls->get<MouseActionControl>()->showContextMenu(event);
}

void GraphicView::saveFlow() {
    if (currentFlow == nullptr || ignoreSaveState) {
        return;
    }

    currentFlow->executors().clear();
    currentFlow->lines().clear();

    auto ctl = controls->get<GraphicObjCreateControl>();

    int nodeId = 1;
    QHash<QSharedPointer<GraphicObject>, int> objIdMap;
    for (auto& node : ctl->nodeObjects) {
        auto executor = qSharedPointerCast<GraphicNode>(node)->toFlowExecutor();
        executor.id = nodeId++;
        currentFlow->executors() << executor;
        objIdMap[node] = executor.id();
    }

    for (auto& link : ctl->linkLines) {
        auto linkLine = qSharedPointerCast<GraphicLinkLine>(link);
        ConfigFlowConnectLine line;
        line.connectFrom = objIdMap[linkLine->linkData->linkFromNode];
        if (line.connectFrom() == 0) {
            continue;
        }
        line.connectFromPIndex = linkLine->linkData->linkFromPointIndex;
        line.connectTo = objIdMap[linkLine->linkData->linkToNode];
        if (line.connectTo() == 0) {
            continue;
        }
        line.connectToPIndex = linkLine->linkData->linkToPointIndex;

        switch (linkLine->linkData->linkFromNode->objectType()) {
            case GraphicObjectType::Node_Event_State: {
                auto eventNode = qSharedPointerCast<NodeEventState>(linkLine->linkData->linkFromNode);
                EventTriggerFunction* trigger;
                bool normalLine = line.connectFromPIndex() == 0;
                if (normalLine) {
                    trigger = &eventNode->eventStateData->eventPropData.normalEvent();
                } else {
                    trigger = &eventNode->eventStateData->eventPropData.errorEvent();
                }
                line.trigger = trigger->triggerFunc();
                line.checkFunc = trigger->checkFunc();
                line.branchId = trigger->branchId();
                line.failBranch = !normalLine;
            }
                break;
            case GraphicObjectType::Node_MultiEvent_State: {
                auto multiEventNode = qSharedPointerCast<NodeMultiEventState>(linkLine->linkData->linkFromNode);
                auto* trigger = &multiEventNode->eventStateData->eventPropData.events()[line.connectFromPIndex()];
                line.trigger = trigger->triggerFunc();
                line.checkFunc = trigger->checkFunc();
                line.branchId = trigger->branchId();
            }
                break;
            case GraphicObjectType::Node_Condition_State: {
                auto conditionNode = qSharedPointerCast<NodeConditionState>(linkLine->linkData->linkFromNode);
                line.branchId = conditionNode->conditionStateData->conditionPropData.branchIds()[line.connectFromPIndex()];
            }
                break;
            case GraphicObjectType::Node_State_Group: {
                auto groupNode = qSharedPointerCast<NodeStateGroup>(linkLine->linkData->linkFromNode);
                if (line.connectFromPIndex() == 0) {
                    line.failBranch = true;
                    line.trigger = groupNode->groupData->groupPropData.errorTriggerFunc();
                } else if (line.connectFromPIndex() == 1) {
                    line.subBranch = true;
                }
            }
                break;
            default:
                break;
        }
        currentFlow->lines() << line;
    }
    emit configChanged();
}

void GraphicView::updateFlow(ConfigFlow *flow, int version) {
    ignoreSaveState = true;
    currentFlow = flow;
    controls->get<GraphicObjCreateControl>()->clearAll();

    if (flow == nullptr) {
        ignoreSaveState = true;
        repaint();
        return;
    }

    QHash<int, QSharedPointer<GraphicNode>> objMap;
    for (auto& executor : flow->executors()) {
        controls->get<GraphicObjCreateControl>()->addObject(executor.itemType(), executor.scenePos());
        auto addObj = qSharedPointerCast<GraphicNode>(controls->get<GraphicObjCreateControl>()->getSelectedNodeObj());
        addObj->fromExecutor(executor);
        if (version == 1) {
            addObj->data->renderPosition += QPointF(5000, 5000);
            addObj->data->renderPosition *= 2;
        }
        objMap[executor.id()] = addObj;
    }
    controls->get<GraphicObjCreateControl>()->cancelObjActiveSelected();

    for (auto& line : flow->lines()) {
        auto lineLine = GraphicLinkLine::create();
        lineLine->linkData->linkFromNode = objMap[line.connectFrom()];
        if (lineLine->linkData->linkFromNode.isNull()) {
            continue;
        }
        lineLine->linkData->linkToNode = objMap[line.connectTo()];
        if (lineLine->linkData->linkToNode.isNull()) {
            continue;
        }
        if (version > 1) {
            lineLine->linkData->linkFromPointIndex = line.connectFromPIndex();
            lineLine->linkData->linkToPointIndex = line.connectToPIndex();
        } else {
            lineLine->linkData->linkFromPointIndex = 0;
            lineLine->linkData->linkToPointIndex = 0;
        }
        lineLine->linkData->isEditing = false;
        switch (lineLine->linkData->linkFromNode->objectType()) {
            case GraphicObjectType::Node_Event_State: {
                auto eventNode = qSharedPointerCast<NodeEventState>(lineLine->linkData->linkFromNode);
                EventTriggerFunction* trigger;
                if (line.failBranch()) {
                    trigger = &eventNode->eventStateData->eventPropData.errorEvent();
                    lineLine->linkData->linkFromPointIndex = 1;
                } else {
                    trigger = &eventNode->eventStateData->eventPropData.normalEvent();
                }
                trigger->triggerFunc = line.trigger();
                trigger->checkFunc = line.checkFunc();
                trigger->branchId = line.branchId();
            }
                break;
            case GraphicObjectType::Node_MultiEvent_State: {
                auto multiEventNode = qSharedPointerCast<NodeMultiEventState>(lineLine->linkData->linkFromNode);
                QList<EventTriggerFunction> &events = multiEventNode->eventStateData->eventPropData.events();
                EventTriggerFunction* trigger;
                if (version == 1) {
                    events.append(EventTriggerFunction());
                    trigger = &events.last();
                    lineLine->linkData->linkFromPointIndex = events.size() - 1;
                } else {
                    while (events.size() < line.connectFromPIndex() + 1) {
                        events.append(EventTriggerFunction());
                    }
                    trigger = &events[line.connectFromPIndex()];
                }
                trigger->triggerFunc = line.trigger();
                trigger->checkFunc = line.checkFunc();
                trigger->branchId = line.branchId();
            }
                break;
            case GraphicObjectType::Node_Condition_State: {
                auto conditionNode = qSharedPointerCast<NodeConditionState>(lineLine->linkData->linkFromNode);
                QList<int>& branchIds = conditionNode->conditionStateData->conditionPropData.branchIds();
                if (version == 1) {
                    branchIds.append(line.branchId());
                    lineLine->linkData->linkFromPointIndex = branchIds.size() - 1;
                } else {
                    while (branchIds.size() < line.connectFromPIndex() + 1) {
                        branchIds.append(-1);
                    }
                    branchIds[line.connectFromPIndex()] = line.branchId();
                }
            }
                break;
            case GraphicObjectType::Node_State_Group: {
                auto groupNode = qSharedPointerCast<NodeStateGroup>(lineLine->linkData->linkFromNode);
                if (line.failBranch()) {
                    groupNode->groupData->groupPropData.errorTriggerFunc = line.trigger();
                }
                if (version == 1) {
                    if (line.failBranch()) {
                        lineLine->linkData->linkFromPointIndex = 0;
                    } else if (line.subBranch()) {
                        lineLine->linkData->linkFromPointIndex = 1;
                    } else {
                        lineLine->linkData->linkFromPointIndex = 2;
                    }
                }
            }
                break;
            default:
                break;
        }
        controls->get<GraphicObjCreateControl>()->linkLines << lineLine;
    }
    controls->get<GraphicLayerControl>()->updateStaticLinkLines(controls->get<GraphicObjCreateControl>()->linkLines);
    controls->get<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node, true);
    controls->get<GraphicLayerControl>()->graphLayerReload();
    repaint();

    ignoreSaveState = false;
}
