#pragma once

#include <qdialog.h>
#include <qlistview.h>

typedef QList<QPair<QString, QString>> StyledMenuItem;
class StyledMenu : public QWidget {
    Q_OBJECT

public:
    explicit StyledMenu(const StyledMenuItem& menus, QWidget *parent = nullptr);

    /**
     * @brief 在指定坐标位置显示
     * @param pos
     * @param selectedIndex 选中项
     */
    void moveShow(const QPoint& pos, int selectedIndex = 0);

signals:
    void onMenuItemClicked(int index);

private:
    StyledMenuItem menus; ///< 菜单项  {icon,text}
    QListView* listView; ///< 菜单项列表

private:
    /**
     * @brief 加载菜单项
     */
    void loadMenu();
};
