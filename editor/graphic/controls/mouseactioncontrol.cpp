#include "mouseactioncontrol.h"
#include "transformcontrol.h"
#include "graphiclayercontrol.h"
#include "graphicobjcreatecontrol.h"

#include "dialogs/nodeedit/normalstatepropeditdlg.h"
#include "dialogs/nodeedit/beginstatepropeditdlg.h"
#include "dialogs/nodeedit/endstatepropeditdlg.h"
#include "dialogs/nodeedit/delaystatepropeditdlg.h"
#include "dialogs/nodeedit/conditionstatepropeditdlg.h"
#include "dialogs/nodeedit/eventstatepropeditdlg.h"
#include "dialogs/nodeedit/multieventstatepropeditdlg.h"
#include "dialogs/nodeedit/groupstatepropeditdlg.h"
#include "dialogs/nodeedit/recoverystatepropeditdlg.h"
#include "dialogs/nodeedit/loopstatepropeditdlg.h"
#include "dialogs/messagedlg.h"

#include "../objects/nodes/nodedelaystate.d.h"
#include "../objects/nodes/nodeconditionstate.d.h"
#include "../objects/nodes/nodeeventstate.d.h"
#include "../objects/nodes/nodemultieventstate.d.h"
#include "../objects/nodes/nodestategroup.d.h"
#include "../objects/nodes/noderecoverystate.d.h"
#include "../objects/nodes/nodeloopstate.d.h"

#include <qevent.h>
#include <qdebug.h>
#include <qmenu.h>
#include <qaction.h>
#include <qshortcut.h>
#include <qtimer.h>

void MouseActionControl::mousePress(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        selectObjPress(e->pos());
        if (objectSelected || linkLineCreating || linkLineSelected) {
            //...
        } else {
            d->getControl<TransformControl>()->moveBegin(e->pos());
        }
    } else if (e->button() == Qt::RightButton) {
        multiSelecting = false;
        d->getControl<GraphicLayerControl>()->beginMultiSelect(e->pos());
    }
}

void MouseActionControl::mouseMove(QMouseEvent *e) {
    auto mousePos = e->pos();
    //qDebug() << "current mouse pos:" << mousePos;

    bool selectTestRequired = true;
    if (e->buttons() & Qt::LeftButton) {
        if (objectSelected) {
            selectObjMove(mousePos);
            selectTestRequired = false;
        } else if (linkLineCreating) {
            linkLineMove(mousePos);
            selectTestRequired = true;
        } else if (linkLineSelected) {
            //...
        } else {
            d->getControl<TransformControl>()->dragMoving(mousePos);
            d->getControl<GraphicLayerControl>()->graphLayerReload();
            selectTestRequired = false;
        }
    } else if (e->buttons() & Qt::RightButton) {
        selectTestRequired = false;
        multiSelecting = true;
        d->getControl<GraphicLayerControl>()->updateMultiSelect(mousePos);
    }

    if (selectTestRequired) {
        auto selectedObj = d->getControl<GraphicObjCreateControl>()->selectTest(mousePos, true);
        if (selectedObj != lastHoverActiveNode) {
            if (lastHoverActiveNode) {
                lastHoverActiveNode->makeLinkPointActive(-1, true);
                lastHoverActiveNode->makeLinkPointActive(-1, false);
                lastHoverActiveNode = nullptr;
                d->getControl<GraphicLayerControl>()->reloadLayer(
                        GraphicLayerType::Layer_Static_Node | GraphicLayerType::Layer_Active_Node);
            }
        }
        if (selectedObj) {
            if (selectedObj->objectType() < GraphicObjectType::Link_Line) { //选中节点
                lastHoverActiveNode = dynamic_cast<const GraphicNode *>(selectedObj);
                //测试是否在输出连接点上
                auto currentMousePoint = d->getGraphicTransform().toRealPoint(mousePos);
                auto inputLinkIndex = dynamic_cast<const GraphicNode *>(selectedObj)->testOnLinkPoint(currentMousePoint, true);
                auto outputLinkIndex = dynamic_cast<const GraphicNode *>(selectedObj)->testOnLinkPoint(
                        currentMousePoint, false);
                lastHoverActiveNode->makeLinkPointActive(inputLinkIndex, true);
                lastHoverActiveNode->makeLinkPointActive(outputLinkIndex, false);
                d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node | GraphicLayerType::Layer_Active_Node);
            }
        }
    }

    d->view->repaint();
}

void MouseActionControl::mouseRelease(QMouseEvent *) {
    if (objectSelected) {
        selectObjRelease();
    } else if (linkLineCreating) {
        linkLineRelease();
    } else if (linkLineSelected) {
        linkLineSelected = false;
    } else {
        positionChanged();
    }
    if (multiSelecting) {
        auto selectArea = d->getControl<GraphicLayerControl>()->getMultiSelectRect();
        d->getControl<GraphicLayerControl>()->endMultiSelect();
        d->getControl<GraphicObjCreateControl>()->createMultiSelect(selectArea);
    }
    d->view->repaint();
}

void MouseActionControl::wheelEvent(QWheelEvent *e) {
    d->getControl<TransformControl>()->scale(e->angleDelta().y() < 0, e->position());
    d->getControl<GraphicLayerControl>()->graphLayerReload();
    d->view->repaint();
}

void MouseActionControl::selectObjPress(const QPoint &mousePos) {
    objectSelected = false;
    linkLineCreating = false;
    lastMousePoint = d->getGraphicTransform().toRealPoint(mousePos);

    const GraphicObject* selectedObj = nullptr;
    if (d->getControl<GraphicObjCreateControl>()->testOnSelectedNode(mousePos)) {
        selectedObj = d->getControl<GraphicObjCreateControl>()->getSelectedNodeObj();
    } else {
        selectedObj = d->getControl<GraphicObjCreateControl>()->selectTest(mousePos);
    }
    if (selectedObj) {
        if (selectedObj->objectType() < GraphicObjectType::Link_Line) { //选中节点
            //测试是否在输出连接点上
            auto node = dynamic_cast<const GraphicNode*>(selectedObj);
            auto linkIndex = node->testOnLinkPoint(lastMousePoint, false);
            if (linkIndex != -1) {
                //检查是否有链接线连接，输出节点只能连接一条线
                if (d->getControl<GraphicObjCreateControl>()->checkIsAnyLinkLineLinkedToNode(node, linkIndex)) {
                    linkIndex = -1;
                }
            }
            if (linkIndex == -1) {
                d->getControl<GraphicObjCreateControl>()->setObjectSelected(selectedObj);
                objectSelected = true;
            } else {
                d->getControl<GraphicObjCreateControl>()->beginActiveLinkLine(selectedObj, linkIndex, lastMousePoint);
                linkLineCreating = true;
            }
        } else if (selectedObj->objectType() == GraphicObjectType::Link_Line) { //选中连接线
            d->getControl<GraphicObjCreateControl>()->setLinkLineSelected(selectedObj);
            linkLineSelected = true;
        }
    } else {
        d->getControl<GraphicObjCreateControl>()->cancelObjActiveSelected();
        d->getControl<GraphicObjCreateControl>()->cancelMultiSelectSelectedState();
    }
}

void MouseActionControl::selectObjMove(const QPoint &mousePos) {
    auto currentMousePoint = d->getGraphicTransform().toRealPoint(mousePos);
    auto delta = currentMousePoint - lastMousePoint;
    d->getControl<GraphicObjCreateControl>()->objTranslate(delta);
    d->view->repaint();
    lastMousePoint = currentMousePoint;
}

void MouseActionControl::selectObjRelease() {
    objectSelected = false;
    d->getControl<GraphicObjCreateControl>()->objTranslateFinished();
}

void MouseActionControl::linkLineMove(const QPoint &mousePos) const {
    auto currentMousePoint = d->getGraphicTransform().toRealPoint(mousePos);
    const GraphicObject* linkToObject = nullptr;
    int linkPointIndex = -1;
    //测试是否在对象的输入连接点上
    auto selectedObj = d->getControl<GraphicObjCreateControl>()->selectTest(mousePos);
    if (selectedObj) {
        if (selectedObj->objectType() < GraphicObjectType::Link_Line) { //选中节点
            auto linkIndex = dynamic_cast<const GraphicNode*>(selectedObj)->testOnLinkPoint(currentMousePoint, true);
            if (linkIndex != -1) {
                linkToObject = selectedObj;
                linkPointIndex = linkIndex;
            }
        }
    }
    d->getControl<GraphicObjCreateControl>()->updateActiveLinkLineToPoint(linkToObject, linkPointIndex, currentMousePoint);
}

void MouseActionControl::linkLineRelease() {
    d->getControl<GraphicObjCreateControl>()->releaseActiveLinkLine();
    linkLineCreating = false;
}

void MouseActionControl::showContextMenu(QContextMenuEvent *event) {
    if (multiSelecting) {
        return;
    }
    auto selectedObject = d->getControl<GraphicObjCreateControl>()->selectTest(event->pos());
    if (selectedObject != nullptr) {
        if (selectedObject->objectType() < GraphicObjectType::Link_Line) { //选中节点
            d->getControl<GraphicObjCreateControl>()->setObjectSelected(selectedObject);
            d->view->update();
            showSelectedObjectMenu(selectedObject, event);
        } else if (selectedObject->objectType() == GraphicObjectType::Link_Line) { //选中连接线
            d->getControl<GraphicObjCreateControl>()->setLinkLineSelected(selectedObject);
            d->view->update();
            showLinkLineMenu(selectedObject, event);
        }
    } else {
        showBlackboardMenu(event);
    }
}

void MouseActionControl::installShortcut() {
    new QShortcut(QKeySequence("Ctrl+C"), d->view, [&] {
        if (!d->multiSelectData.selectedNodes.isEmpty()) {
            copyNodeObject(d->multiSelectData.selectedNodes);
        } else {
            copyNodeObject({
                dynamic_cast<const GraphicNode*>(d->getControl<GraphicObjCreateControl>()->getSelectedNodeObj())
            });
        }
    });

    new QShortcut(QKeySequence("Ctrl+V"), d->view, [&] {
        pasteNodeObject(d->view->mapFromGlobal(QCursor::pos()));
    });

    new QShortcut(QKeySequence("Del"), d->view, [&] {
        if (!d->multiSelectData.selectedNodes.isEmpty()) {
            d->getControl<GraphicObjCreateControl>()->removeMultiSelectedObjects();
            d->view->repaint();
        } else {
            auto activeNode = d->getControl<GraphicObjCreateControl>()->getSelectedNodeObj();
            if (activeNode != nullptr) {
                deleteNodeObject(activeNode);
                return;
            }
        }
        auto activeLinkLine = d->getControl<GraphicObjCreateControl>()->getSelectedLinkLine();
        if (activeLinkLine != nullptr) {
            removeLinkLine(activeLinkLine);
            return;
        }
    });

    new QShortcut(QKeySequence("Ctrl+D"), d->view, [&] {
        editNodeObject(d->getControl<GraphicObjCreateControl>()->getSelectedNodeObj());
    });

    new QShortcut(QKeySequence("Ctrl+Z"), d->view, [&] {
        if (!d->graphicObjects.canUndo()) {
            return;
        }
        if (d->graphicObjects.command(d->graphicObjects.index() - 1)->text() == "DocumentLoad") {
            return;
        }
        if (d->graphicObjects.command(d->graphicObjects.index() - 1)->text() == "CannotUndoOperate") {
            MessageDlg::showMessage(tr("无法撤销的操作"), tr("这是一个未解决的问题，编辑属性造成的连接线删除无法撤销。"));
            return;
        }
        d->graphicObjects.undo();
        d->getControl<GraphicLayerControl>()->graphLayerReload();
        d->getControl<GraphicObjCreateControl>()->graphicObjectChanged();
        d->view->update();
        QTimer::singleShot(20, this, [&] {
            d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Link | GraphicLayerType::Layer_Static_Link);
            d->view->update();
        });
    });

    new QShortcut(QKeySequence("Ctrl+Shift+Z"), d->view, [&] {
        if (!d->graphicObjects.canRedo()) {
            return;
        }
        d->graphicObjects.redo();
        d->getControl<GraphicLayerControl>()->graphLayerReload();
        d->getControl<GraphicObjCreateControl>()->graphicObjectChanged();
        d->view->update();
        QTimer::singleShot(20, this, [&] {
            d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Link | GraphicLayerType::Layer_Static_Link);
            d->view->update();
        });
    });
}

void MouseActionControl::clearTempData() {
    lastHoverActiveNode = nullptr;
}

void MouseActionControl::showSelectedObjectMenu(const GraphicObject* obj, QContextMenuEvent *event) {
    QMenu menu(d->view);
    QList<QAction *> actions;
    actions << menu.addAction(tr("复制"), [this, obj] { copyNodeObject({ dynamic_cast<const GraphicNode*>(obj) }); }, QKeySequence("Ctrl+C"));
    actions << menu.addAction(tr("粘贴"), [this, event] { pasteNodeObject(event->pos()); }, QKeySequence("Ctrl+V"));
    actions << menu.addAction(tr("删除"), [this, obj] { deleteNodeObject(obj); }, QKeySequence("Del"));
    menu.addSeparator();
    actions << menu.addAction(tr("编辑"), [this, obj] { editNodeObject(obj); }, QKeySequence("Ctrl+D"));
    menu.exec(event->globalPos());
}

void MouseActionControl::showLinkLineMenu(const GraphicObject* obj, QContextMenuEvent *event) const {
    QMenu menu(d->view);
    QList<QAction *> actions;
    actions << menu.addAction(tr("删除"), [this, obj] { removeLinkLine(obj); }, QKeySequence("Del"));
    menu.exec(event->globalPos());
}

void MouseActionControl::showBlackboardMenu(QContextMenuEvent *event) const {
    QMenu menu(d->view);
    QList<QAction *> actions;
    actions << menu.addAction(tr("粘贴"), [this, event] { pasteNodeObject(event->pos()); }, QKeySequence("Ctrl+V"));
    menu.addSeparator();
    actions << menu.addAction(tr("开始状态"));
    actions << menu.addAction(tr("结束状态"));
    actions << menu.addAction(tr("普通状态"));
    actions << menu.addAction(tr("延时状态"));
    actions << menu.addAction(tr("事件触发状态"));
    actions << menu.addAction(tr("复合事件状态"));
    actions << menu.addAction(tr("条件分支状态"));
    actions << menu.addAction(tr("子状态组"));
    actions << menu.addAction(tr("恢复点"));
    actions << menu.addAction(tr("循环执行状态"));

    auto selectedAction = menu.exec(event->globalPos());
    int actionIndex = actions.indexOf(selectedAction);
    if (actionIndex > 0) {
        d->getControl<GraphicObjCreateControl>()->addObject(GraphicObjectType(actionIndex - 1), d->getGraphicTransform().toRealPoint(event->pos()));
    }
}

void MouseActionControl::editNodeObject(const GraphicObject* obj) const {
    if (obj == nullptr) {
        return;
    }

    const auto& showPropertyDlg = [&] (BasePropertyEditDlg& dlg,
                                       const std::function<void(const QSharedPointer<GraphicNodeData>&)>& exDataRead = nullptr,
                                       const std::function<void(QSharedPointer<GraphicNodeData>&)>& exDataWrite = nullptr)
    {
        auto objData = qSharedPointerCast<GraphicNodeData>(obj->data);
        dlg.setData(objData->propData);
        if (exDataRead) {
            exDataRead(objData);
        }
        auto exitCode = dlg.exec();
        if (exitCode == QDialog::Accepted) {
            objData->propData = dlg.getEditData();
            if (exDataWrite) {
                exDataWrite(objData);
            }
            objData->isChanged = true;
            d->getControl<GraphicObjCreateControl>()->removeLinkLineOutOfIndex(obj);
            d->getControl<GraphicObjCreateControl>()->graphicObjectChanged();
            d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Node | GraphicLayerType::Layer_Active_Link);
            d->view->repaint();
        }
    };

    switch (obj->objectType()) {
        case GraphicObjectType::Node_Begin_State: {
            BeginStatePropEditDlg dlg;
            showPropertyDlg(dlg);
        }
            break;
        case GraphicObjectType::Node_End_State: {
            EndStatePropEditDlg dlg;
            showPropertyDlg(dlg);
        }
            break;
        case GraphicObjectType::Node_Normal_State: {
            NormalStatePropEditDlg dlg;
            showPropertyDlg(dlg);
        }
            break;
        case GraphicObjectType::Node_Delay_State: {
            DelayStatePropEditDlg dlg;
            showPropertyDlg(dlg, [&] (const QSharedPointer<GraphicNodeData>& objData) {
                dlg.setExData(qSharedPointerCast<NodeDelayStateData>(objData)->delayPropData);
            }, [&] (QSharedPointer<GraphicNodeData>& objData) {
                qSharedPointerCast<NodeDelayStateData>(objData)->delayPropData = dlg.getExEditData();
            });
        }
            break;
        case GraphicObjectType::Node_Event_State: {
            EventStatePropEditDlg dlg;
            showPropertyDlg(dlg, [&] (const QSharedPointer<GraphicNodeData>& objData) {
                dlg.setExData(qSharedPointerCast<NodeEventStateData>(objData)->eventPropData);
            }, [&] (QSharedPointer<GraphicNodeData>& objData) {
                qSharedPointerCast<NodeEventStateData>(objData)->eventPropData = dlg.getExEditData();
            });
        }
            break;
        case GraphicObjectType::Node_MultiEvent_State: {
            MultiEventStatePropEditDlg dlg;
            showPropertyDlg(dlg, [&] (const QSharedPointer<GraphicNodeData>& objData) {
                dlg.setExData(qSharedPointerCast<NodeMultiEventStateData>(objData)->eventPropData);
            }, [&] (QSharedPointer<GraphicNodeData>& objData) {
                qSharedPointerCast<NodeMultiEventStateData>(objData)->eventPropData = dlg.getExEditData();
            });
        }
            break;
        case GraphicObjectType::Node_Condition_State: {
            ConditionStatePropEditDlg dlg;
            showPropertyDlg(dlg, [&] (const QSharedPointer<GraphicNodeData>& objData) {
                dlg.setExData(qSharedPointerCast<NodeConditionStateData>(objData)->conditionPropData);
            }, [&] (QSharedPointer<GraphicNodeData>& objData) {
                qSharedPointerCast<NodeConditionStateData>(objData)->conditionPropData = dlg.getExEditData();
            });
        }
            break;
        case GraphicObjectType::Node_State_Group: {
            GroupStatePropEditDlg dlg;
            showPropertyDlg(dlg, [&] (const QSharedPointer<GraphicNodeData>& objData) {
                dlg.setExData(qSharedPointerCast<NodeStateGroupData>(objData)->groupPropData);
            }, [&] (QSharedPointer<GraphicNodeData>& objData) {
                qSharedPointerCast<NodeStateGroupData>(objData)->groupPropData = dlg.getExEditData();
            });
        }
            break;
        case GraphicObjectType::Node_Recovery_State: {
            RecoveryStatePropEditDlg dlg;
            showPropertyDlg(dlg, [&] (const QSharedPointer<GraphicNodeData>& objData) {
                dlg.setExData(qSharedPointerCast<NodeRecoveryStateData>(objData)->recoveryPropData);
            }, [&] (QSharedPointer<GraphicNodeData>& objData) {
                qSharedPointerCast<NodeRecoveryStateData>(objData)->recoveryPropData = dlg.getExEditData();
            });
        }
            break;
        case GraphicObjectType::Node_Loop_State: {
            LoopStatePropEditDlg dlg;
            showPropertyDlg(dlg, [&] (const QSharedPointer<GraphicNodeData>& objData) {
                dlg.setExData(qSharedPointerCast<NodeLoopStateData>(objData)->loopStatePropData);
            }, [&] (QSharedPointer<GraphicNodeData>& objData) {
                qSharedPointerCast<NodeLoopStateData>(objData)->loopStatePropData = dlg.getExEditData();
            });
            break;
        }
        default:
            break;
    }
}

void MouseActionControl::copyNodeObject(const QList<const GraphicNode*>& objs) {
    preCopyObjects.clear();
    QHash<const GraphicObject*, const GraphicNode*> srcToCloneObjs;
    for (auto node : objs) {
        if (node == nullptr) {
            continue;
        }
        preCopyObjects.append(QSharedPointer<GraphicObject>(node->clone()));
        srcToCloneObjs[node] = dynamic_cast<GraphicNode*>(preCopyObjects.last().data());
    }
    //copy link line
    auto linkLines = GraphicObject::getVisibleObjects<GraphicLinkLine>(&d->graphicObjects);
    for (const auto& linkLine : linkLines) {
        if (srcToCloneObjs.contains(linkLine->linkData->linkFromNode) &&
            srcToCloneObjs.contains(linkLine->linkData->linkToNode))
        {
            auto newLinkLine = dynamic_cast<GraphicLinkLine*>(linkLine->clone());
            newLinkLine->linkData->linkFromNode = srcToCloneObjs[linkLine->linkData->linkFromNode];
            newLinkLine->linkData->linkToNode = srcToCloneObjs[linkLine->linkData->linkToNode];
            preCopyObjects.append(QSharedPointer<GraphicObject>(newLinkLine));
        }
    }
}

void MouseActionControl::pasteNodeObject(const QPoint& mousePos) const {
    if (preCopyObjects.isEmpty()) {
        return;
    }
    d->getControl<GraphicObjCreateControl>()->copyNodeToMousePoint(preCopyObjects, mousePos);
    d->view->repaint();
}

void MouseActionControl::deleteNodeObject(const GraphicObject* obj) const {
    if (obj == nullptr) {
        return;
    }
    d->getControl<GraphicObjCreateControl>()->removeNodeObject(obj);
    d->view->repaint();
}

void MouseActionControl::removeLinkLine(const GraphicObject* obj) const {
    d->getControl<GraphicObjCreateControl>()->removeLinkLine(obj);
    d->view->update();
}
