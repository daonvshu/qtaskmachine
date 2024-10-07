#include "mouseactioncontrol.h"
#include "transformcontrol.h"
#include "graphiclayercontrol.h"
#include "graphicobjcreatecontrol.h"

#include <qevent.h>
#include <qdebug.h>

void MouseActionControl::mousePress(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        mousePressPoint = e->pos();
        selectObjPress(e->pos());
        d->getControl<TransformControl>()->moveBegin(e->pos());
    }
}

void MouseActionControl::mouseMove(QMouseEvent *e) {
    auto mousePos = e->pos();

    if (e->buttons() & Qt::LeftButton) {
        if (objectSelected) {
            selectObjMove(mousePos);
        } else {
            d->getControl<TransformControl>()->dragMoving(mousePos);
            d->getControl<GraphicLayerControl>()->graphLayerReload();
        }
    }

    d->view->repaint();
}

void MouseActionControl::mouseRelease(QMouseEvent *) {
    if (objectSelected) {
        selectObjRelease();
    }
    d->view->repaint();
}

void MouseActionControl::wheelEvent(QWheelEvent *e) {
    d->getControl<TransformControl>()->scale(e->angleDelta().y() < 0, e->position());
    d->getControl<GraphicLayerControl>()->graphLayerReload();
    d->view->repaint();
}

void MouseActionControl::selectObjPress(const QPoint &mousePos) {
    auto selectedObj = d->getControl<GraphicObjCreateControl>()->selectTest(mousePos);
    objectSelected = !selectedObj.isNull();
    lastMousePoint = d->getGraphicTransform().toRealPoint(mousePos);
}

void MouseActionControl::selectObjMove(const QPoint &mousePos) {
    auto currentMousePoint = d->getGraphicTransform().toRealPoint(mousePos);
    auto delta = currentMousePoint - lastMousePoint;
    d->getControl<GraphicObjCreateControl>()->translate(delta);
    d->view->repaint();
    lastMousePoint = currentMousePoint;
}

void MouseActionControl::selectObjRelease() {
    objectSelected = false;
}