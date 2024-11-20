#pragma once

#include <qwidget.h>

struct ConfigFlow;
class GraphicControlWrapper;
class GraphicView : public QWidget {
    Q_OBJECT

public:
    explicit GraphicView(QWidget *parent = nullptr);

    void updateFlow(ConfigFlow* flow);

signals:
    void configChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    GraphicControlWrapper* controls;
    QColor backgroundColor;

    ConfigFlow* currentFlow = nullptr;
    bool ignoreSaveState = false;

private slots:
    void saveFlow();
    void savePosition();
};
