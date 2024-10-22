#pragma once

#include <qobject.h>
#include <qevent.h>

#include "../graphiccontrol.h"
#include "../graphicobject.h"

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

    /**
     * @brief 右键菜单
     * @param event
     */
    void showContextMenu(QContextMenuEvent *event);

    /**
     * @brief 注册快捷键
     */
    void installShortcut();

private:
    bool objectSelected = false;
    bool linkLineCreating = false;
    bool linkLineSelected = false;
    QPointF lastMousePoint;
    QSharedPointer<class GraphicNode> lastHoverActiveNode;
    QWeakPointer<GraphicObject> preCopyObject;

private:
    void selectObjPress(const QPoint& mousePos);
    void selectObjMove(const QPoint& mousePos);
    void selectObjRelease();

    void linkLineMove(const QPoint& mousePos);
    void linkLineRelease();

    /**
     * @brief 显示节点的右键菜单
     * @param obj
     * @param event
     */
    void showSelectedObjectMenu(const QSharedPointer<GraphicObject>& obj, QContextMenuEvent *event);

    /**
     * @brief 显示链接线的右键菜单
     * @param obj
     * @param event
     */
    void showLinkLineMenu(const QSharedPointer<GraphicObject>& obj, QContextMenuEvent *event);

    /**
     * @brief 显示背景的右键菜单
     * @param event
     */
    void showBlackboardMenu(QContextMenuEvent *event);

    /**
     * @brief 编辑节点对象
     * @param obj
     */
    void editNodeObject(const QSharedPointer<GraphicObject>& obj);

    /**
     * @brief 预拷贝对象
     * @param obj
     */
    void copyNodeObject(const QSharedPointer<GraphicObject>& obj);

    /**
     * @brief 粘贴对象
     * @param mousePos
     */
    void pasteNodeObject(const QPoint& mousePos);

    /**
     * @brief 删除对象
     * @param obj
     */
    void deleteNodeObject(const QSharedPointer<GraphicObject>& obj);

    /**
     * @brief 删除链接线
     * @param obj
     */
    void removeLinkLine(const QSharedPointer<GraphicObject>& obj);
};
