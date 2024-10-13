#include "mouseactioncontrol.h"
#include "transformcontrol.h"
#include "graphiclayercontrol.h"
#include "graphicobjcreatecontrol.h"

#include "dialogs/nodeedit/normalstatepropeditdlg.h"
#include "dialogs/nodeedit/beginstatepropeditdlg.h"
#include "dialogs/nodeedit/endstatepropeditdlg.h"
#include "dialogs/nodeedit/delaystatepropeditdlg.h"

#include "../objects/nodes/nodedelaystate.d.h"

#include <qevent.h>
#include <qdebug.h>
#include <qmenu.h>
#include <qaction.h>

void MouseActionControl::mousePress(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        mousePressPoint = e->pos();
        selectObjPress(e->pos());
        if (objectSelected) {

        } else {
            d->getControl<TransformControl>()->moveBegin(e->pos());
        }
    }
}

void MouseActionControl::mouseMove(QMouseEvent *e) {
    auto mousePos = e->pos();

    if (e->buttons() & Qt::LeftButton) {
        if (objectSelected) {
            selectObjMove(mousePos);
        } else if (linkLineCreating) {
            linkLineMove(mousePos);
        } else {
            d->getControl<TransformControl>()->dragMoving(mousePos);
            d->getControl<GraphicLayerControl>()->graphLayerReload();
        }
    }

    auto selectedObj = d->getControl<GraphicObjCreateControl>()->selectTest(mousePos, true);
    if (selectedObj != lastHoverActiveNode) {
        if (lastHoverActiveNode) {
            lastHoverActiveNode->makeLinkPointActive(-1, true);
            lastHoverActiveNode->makeLinkPointActive(-1, false);
            lastHoverActiveNode = nullptr;
            d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node | GraphicLayerType::Layer_Active_Node);
        }
    }
    if (selectedObj) {
        if (selectedObj->objectType() <= GraphicObjectType::Node_Recovery_State) { //选中节点
            lastHoverActiveNode = qSharedPointerCast<GraphicNode>(selectedObj);
            //测试是否在输出连接点上
            auto currentMousePoint = d->getGraphicTransform().toRealPoint(mousePos);
            auto inputLinkIndex = qSharedPointerCast<GraphicNode>(selectedObj)->testOnLinkPoint(currentMousePoint, true);
            auto outputLinkIndex = qSharedPointerCast<GraphicNode>(selectedObj)->testOnLinkPoint(currentMousePoint, false);
            lastHoverActiveNode->makeLinkPointActive(inputLinkIndex, true);
            lastHoverActiveNode->makeLinkPointActive(outputLinkIndex, false);
            d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Static_Node | GraphicLayerType::Layer_Active_Node);
        }
    }

    d->view->repaint();
}

void MouseActionControl::mouseRelease(QMouseEvent *) {
    if (objectSelected) {
        selectObjRelease();
    } else if (linkLineCreating) {
        linkLineRelease();
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

    auto selectedObj = d->getControl<GraphicObjCreateControl>()->selectTest(mousePos);
    if (selectedObj) {
        if (selectedObj->objectType() <= GraphicObjectType::Node_Recovery_State) { //选中节点
            //测试是否在输出连接点上
            auto linkIndex = qSharedPointerCast<GraphicNode>(selectedObj)->testOnLinkPoint(lastMousePoint, false);
            if (linkIndex == -1) {
                d->getControl<GraphicObjCreateControl>()->setObjectSelected(selectedObj);
                objectSelected = true;
            } else {
                d->getControl<GraphicObjCreateControl>()->beginActiveLinkLine(selectedObj, linkIndex, lastMousePoint);
                linkLineCreating = true;
            }
        } else if (selectedObj->objectType() == GraphicObjectType::Link_Line) { //选中连接线

        }
    } else {
        d->getControl<GraphicObjCreateControl>()->cancelObjActiveSelected();
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
}

void MouseActionControl::linkLineMove(const QPoint &mousePos) {
    auto currentMousePoint = d->getGraphicTransform().toRealPoint(mousePos);
    QSharedPointer<GraphicObject> linkToObject;
    int linkPointIndex = -1;
    //测试是否在对象的输入连接点上
    auto selectedObj = d->getControl<GraphicObjCreateControl>()->selectTest(mousePos);
    if (selectedObj) {
        if (selectedObj->objectType() <= GraphicObjectType::Node_Recovery_State) { //选中节点
            auto linkIndex = qSharedPointerCast<GraphicNode>(selectedObj)->testOnLinkPoint(currentMousePoint, true);
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
    auto selectedObject = d->getControl<GraphicObjCreateControl>()->selectTest(event->pos());
    if (!selectedObject.isNull()) {
        d->getControl<GraphicObjCreateControl>()->setObjectSelected(selectedObject);
        showSelectedObjectMenu(selectedObject, event);
    } else {
        showBlackboardMenu(event);
    }
}

void MouseActionControl::showSelectedObjectMenu(const QSharedPointer<GraphicObject> &obj, QContextMenuEvent *event) {
    QMenu menu(d->view);
    QList<QAction *> actions;
    actions << menu.addAction(tr("复制 (Ctrl+C)"));
    actions << menu.addAction(tr("粘贴 (Ctrl+V)"));
    actions << menu.addAction(tr("删除 (Del)"));
    menu.addSeparator();
    actions << menu.addAction(tr("编辑"));

    auto selectedAction = menu.exec(event->globalPos());
    int actionIndex = actions.indexOf(selectedAction);
    if (actionIndex != -1) {
        if (actionIndex == 0) {

        } else if (actionIndex == 1) {

        } else if (actionIndex == 2) {

        } else if (actionIndex == 3) {

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
                    d->getControl<GraphicLayerControl>()->reloadLayer(GraphicLayerType::Layer_Active_Node | GraphicLayerType::Layer_Active_Link);
                    d->view->repaint();
                }
            };

            auto objectType = obj->objectType();
            switch (objectType) {
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
                case GraphicObjectType::Link_Line:
                    break;
            }
        }
    }
}

void MouseActionControl::showBlackboardMenu(QContextMenuEvent *event) {
    QMenu menu(d->view);
    QList<QAction *> actions;
    actions << menu.addAction(tr("粘贴 (Ctrl+V)"));
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

    auto selectedAction = menu.exec(event->globalPos());
    int actionIndex = actions.indexOf(selectedAction);
    if (actionIndex != -1) {
        if (actionIndex == 0) {

        } else {
            d->getControl<GraphicObjCreateControl>()->addObject(GraphicObjectType(actionIndex - 1), event->pos());
        }
    }
}
