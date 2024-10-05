#include "graphicview.h"

#include "graphiccontrol.h"

#include "controls/graphiclayercontrol.h"
#include "controls/mouseactioncontrol.h"
#include "controls/transformcontrol.h"
#include "controls/graphicobjcreatecontrol.h"

#include <qpainter.h>
#include <qmenu.h>
#include <qaction.h>

GraphicView::GraphicView(QWidget *parent)
    : QWidget(parent)
    , backgroundColor(0x35363C)
{
    controls = new GraphicControlWrapper(this, this);
    controls->add<GraphicLayerControl>();
    controls->add<MouseActionControl>();
    controls->add<TransformControl>();
    controls->add<GraphicObjCreateControl>();

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
    QMenu menu(this);
    QList<QAction*> actions;
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
        controls->get<GraphicObjCreateControl>()->addObject(GraphicObjectType(actionIndex), event->pos());
    }
}
