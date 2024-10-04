#pragma once

#include <qwidget.h>

class GraphicControlWrapper;
class GraphicView : public QWidget {
public:
    explicit GraphicView(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    GraphicControlWrapper* controls;
    QColor backgroundColor;
};
