#pragma once

#include <qobject.h>
#include <qevent.h>

#include "../graphiccontrol.h"

class MouseActionControl : public GraphicControl {
public:
    using GraphicControl::GraphicControl;

    /**
     * @brief 鼠标点击
     * @param e
     */
    void mousePress(QMouseEvent* e);

    /**
     * @brief 鼠标移动
     * @param e
     */
    void mouseMove(QMouseEvent* e);

    /**
     * @brief 鼠标释放
     * @param e
     */
    void mouseRelease(QMouseEvent* e);

    /**
     * @brief 鼠标滚轮
     * @param e
     */
    void wheelEvent(QWheelEvent* e);

private:
    bool preSelect = false;
    bool dragging = false;
    bool selecting = false;
    QPointF lastMousePoint;
    QPointF mousePressPoint;

private:
    void selectObjPress(const QPoint& mousePos);
    void selectObjMove(const QPoint& mousePos);
    void selectObjRelease();
};
