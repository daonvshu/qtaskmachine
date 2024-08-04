#include "flowchartwidget.h"

#include "flowchartitemdata.h"
#include "elements/fcexecutoritem.h"
#include "elements/fcconnectline.h"
#include "elements/fcbranchitem.h"

#include <QGraphicsSceneDragDropEvent>
#include <qevent.h>
#include <qdebug.h>

FlowChartWidget::FlowChartWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //setBackgroundBrush(Qt::transparent);
    setDragMode(QGraphicsView::ScrollHandDrag);

    scene = new FlowChartScene;
    scene->setSceneRect(-10000, -10000, 10000, 10000);
    setScene(scene);

    viewport()->setCursor(Qt::SizeAllCursor);

    connect(scene, &QGraphicsScene::selectionChanged, this, [&] {
        auto items = scene->selectedItems();
        if (items.isEmpty()) {
            emit nodeSelected(nullptr);
        } else {
            emit nodeSelected(dynamic_cast<FlowChartExecutorItem*>(items.first()));
        }
    });
}

void FlowChartWidget::removeSelected() {
    auto selected = scene->selectedItems();
    for (const auto& item : selected) {
        if (auto executorItem = dynamic_cast<FlowChartExecutorItem*>(item)) {
            executorItem->disconnectLines();
        } else if (auto line = dynamic_cast<FcConnectLine*>(item)) {
            if (line->connectTo != nullptr) {
                line->connectTo->disconnectLine(line);
            }
            if (line->connectFrom != nullptr) {
                line->connectFrom->disconnectLine(line);
            }
        }
        scene->removeItem(item);
    }
}

QGraphicsScene *FlowChartWidget::getScene() const {
    return scene;
}

QList<QGraphicsItem *> FlowChartWidget::getAllItems() const {
    return scene->items();
}

void FlowChartWidget::updateExecutorItemName(const QString &name, qint64 taskId) {
    auto curItems = items();
    for (const auto& item : curItems) {
        if (auto executorItem = dynamic_cast<FcExecutorItem*>(item)) {
            if (executorItem->itemData.taskId == taskId) {
                executorItem->itemData.text = name;
                executorItem->updateGeometry(scene);
                break;
            }
        }
    }
}

void FlowChartWidget::resetFlowsId() {
    auto curItems = items();
    int nodeId = 0;
    for (const auto& item : curItems) {
        if (auto ei = dynamic_cast<FcExecutorItem*>(item)) {
            ei->itemData.flowId = nodeId++;
        } else if (auto ci = dynamic_cast<FcConditionalItem*>(item)) {
            ci->flowId = nodeId++;
        }
    }
}

int FlowChartWidget::getSelectedNodeId() const {
    int id = -1;
    auto items = scene->selectedItems();
    if (!items.isEmpty()) {
        for (const auto& i : items) {
            if (auto ei = dynamic_cast<FcExecutorItem*>(i)) {
                id = ei->itemData.flowId;
                break;
            }
        }
    }
    return id;
}

void FlowChartWidget::clear() {
    scene->clear();
}

FlowChartScene::FlowChartScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

void FlowChartScene::dropEvent(QGraphicsSceneDragDropEvent *event) {
    if (acceptDrag) {
        event->acceptProposedAction();
        auto mimeData = event->mimeData();
        auto nodeType = FlowChartMimeData::unpack(mimeData);

        switch (nodeType) {
            case FlowChartNodeType::Node_Normal: {
                auto item = new FcExecutorItem(FlowChartItemData());
                item->setPos(event->scenePos(), this);
                addItem(item);
            }
                break;
            case FlowChartNodeType::Node_Condition: {
                auto item = new FcConditionalItem;
                item->setPos(event->scenePos(), this);
                addItem(item);
            }
                break;
        }
    }
}

void FlowChartScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    auto mimeData = event->mimeData();
    if (mimeData->hasFormat(FlowChartMimeData::mimeType())) {
        event->acceptProposedAction();
        acceptDrag = true;
    }
}

void FlowChartScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
    acceptDrag = false;
}

void FlowChartScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
    if (acceptDrag) { //TODO:绘制辅助线
        auto mimeData = event->mimeData();
        auto flowData = FlowChartMimeData::unpack(mimeData);

    }
}

void FlowChartScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);

    auto curItems = items(event->pos());
    for (const auto& item : curItems) {
        if (auto i = dynamic_cast<FlowChartExecutorItem*>(item)) {
            auto p = i->testInConnectPoint(event->pos());
            if (!p.isNull()) {
                if (dynamic_cast<FcExecutorItem*>(item)) {
                    if (i->getConnectedLineSize() >= 1) { //限制节点只能连接一个
                        break;
                    }
                }

                isConnectPointClicked = true;
                lastConnectItem = i;

                tmpConnectLine = new FcConnectLine;
                tmpConnectLine->connectFrom = i;
                tmpConnectLine->ctl.targetBegin = p;
                addItem(tmpConnectLine);
                break;
            }
        }
    }

    if (isConnectPointClicked) {
        setCursor(Qt::ArrowCursor);
    } else {
        setCursor(Qt::SizeAllCursor);
    }
}

void FlowChartScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseMoveEvent(event);

    auto pos = event->pos();
    if (isConnectPointClicked) {
        auto item = itemAt(pos, QTransform());
        if (item != lastConnectItem) {
            if (lastHoverItem != nullptr) {
                lastHoverItem->setIsHoverState(false);
            }
            if (auto fcItem = dynamic_cast<FlowChartExecutorItem*>(item)) {
                lastHoverItem = fcItem;
                lastHoverItem->setIsHoverState(true);
            }
        }

        lastConnectToItem = nullptr;
        auto endPoint = event->pos();
        if (lastHoverItem != nullptr && lastHoverItem != lastConnectItem) {
            auto connectPoint = lastHoverItem->testInConnectPoint(endPoint);
            if (!connectPoint.isNull()) {
                endPoint = connectPoint;
                lastConnectToItem = lastHoverItem;
            }
        }

        tmpConnectLine->ctl.targetEnd = endPoint;
        tmpConnectLine->connectTo = lastConnectToItem;
        tmpConnectLine->refreshConnectPath(true);
    }
}

void FlowChartScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseReleaseEvent(event);
    setCursor(Qt::SizeAllCursor);
    if (isConnectPointClicked) {
        if (lastConnectItem != nullptr && lastConnectToItem != nullptr) {
            addConnectLineFor(lastConnectItem, lastConnectToItem, tmpConnectLine);
        } else {
            removeItem(tmpConnectLine);
        }
    }
    isConnectPointClicked = false;
    lastHoverItem = nullptr;
}

void FlowChartScene::setCursor(const QCursor &cursor) {
    views().first()->viewport()->setCursor(cursor);
}

void FlowChartScene::addConnectLineFor(FlowChartExecutorItem *from, FlowChartExecutorItem *to, FcConnectLine *line) {
    line->connectFrom = from;
    line->connectTo = to;
    from->setConnectToLine(line);
    to->setConnectFromLine(line);
}
