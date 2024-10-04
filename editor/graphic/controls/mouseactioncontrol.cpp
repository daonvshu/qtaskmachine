#include "mouseactioncontrol.h"
#include "transformcontrol.h"
#include "graphiclayercontrol.h"

#include <qevent.h>

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
        if (mousePressPoint != mousePos) {
            selectObjMove(mousePos);
            d->getControl<TransformControl>()->dragMoving(mousePos);
            d->getControl<GraphicLayerControl>()->graphLayerReload();
        }
    }

    d->view->repaint();
}

void MouseActionControl::mouseRelease(QMouseEvent *) {
    selectObjRelease();
    d->view->repaint();
}

void MouseActionControl::wheelEvent(QWheelEvent *e) {
    d->getControl<TransformControl>()->scale(e->angleDelta().y() < 0, e->position());
    d->getControl<GraphicLayerControl>()->graphLayerReload();
    d->view->repaint();
}

void MouseActionControl::selectObjPress(const QPoint &mousePos) {
    preSelect = true; //测试是否是鼠标左键选择
    dragging = false;
}

void MouseActionControl::selectObjMove(const QPoint &) {
    dragging = true;
}

void MouseActionControl::selectObjRelease() {
    if (dragging) {

    }
    preSelect = false;
}