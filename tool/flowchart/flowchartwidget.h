#pragma once

#include <qobject.h>
#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>

class FlowChartExecutorItem;
class FcConnectLine;
class FlowChartScene;
class FlowChartWidget : public QGraphicsView {
    Q_OBJECT

public:
    explicit FlowChartWidget(QWidget *parent = nullptr);

    void removeSelected();

    QGraphicsScene* getScene() const;

    QList<QGraphicsItem*> getAllItems() const;

    void updateExecutorItemName(const QString& name, qint64 taskId);

    void resetFlowsId();

    int getSelectedNodeId() const;

    void clear();

signals:
    void nodeSelected(QGraphicsItem* item);

private:
    FlowChartScene* scene;
};

class FlowChartScene : public QGraphicsScene {
public:
    explicit FlowChartScene(QObject *parent = nullptr);

    static void addConnectLineFor(FlowChartExecutorItem* from, FlowChartExecutorItem* to, FcConnectLine* line);

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;

    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;

    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool acceptDrag = false;
    bool isConnectPointClicked = false;

    FlowChartExecutorItem* lastConnectItem = nullptr;
    FlowChartExecutorItem* lastConnectToItem = nullptr;

    FlowChartExecutorItem* lastHoverItem = nullptr;
    FcConnectLine* tmpConnectLine;

private:
    void setCursor(const QCursor &cursor);
};