#include "styledmenu.h"

#include <qstandarditemmodel.h>
#include <qstyleoption.h>
#include <qpainter.h>
#include <qboxlayout.h>
#include <qgraphicseffect.h>
#include <qdebug.h>

StyledMenu::StyledMenu(const StyledMenuItem& menus, QWidget *parent)
    : QWidget(parent)
    , menus(menus)
    , listView(nullptr)
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);

    setStyleSheet("font-size：12px");

    loadMenu();
}

#define CONTENT_PADDING 5

void StyledMenu::moveShow(const QPoint &pos, int selectedIndex) {
    listView->setCurrentIndex(dynamic_cast<QStandardItemModel*>(listView->model())->index(selectedIndex, 0));
    move(pos - QPoint(CONTENT_PADDING, CONTENT_PADDING));

    int requiredWidth = 0;
    auto fm = fontMetrics();
    for (const auto& menu : menus) {
        int minWidth = fm.horizontalAdvance(menu.second) + QPixmap(menu.first).width() + 8;
        requiredWidth = qMax(requiredWidth, minWidth);
    }
    requiredWidth = qMax(requiredWidth, 102);
    listView->setFixedWidth(requiredWidth);
    requiredWidth += 2 * CONTENT_PADDING;
    int requiredHeight = 32 * menus.size() + 2 * CONTENT_PADDING;
    setFixedSize({ requiredWidth, requiredHeight });

    //qDebug() << "fix size: width:" << requiredWidth << "height:" << requiredHeight;

    show();
}

void StyledMenu::loadMenu() {
    auto boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(CONTENT_PADDING, CONTENT_PADDING, CONTENT_PADDING, CONTENT_PADDING);

    listView = new QListView(this);
    listView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    listView->setEditTriggers(QListView::NoEditTriggers);
    listView->setStyleSheet(QString(""
                                    "QListView{background:#26282D;border:none;border-radius:4px;outline:none;}"
                                    "QListView::item{color:white;border:none;border-radius:4px;}"
                                    "QListView::item:hover{color:#53A9FF;background:#4A4B53;}"
                                    ));
    connect(listView, &QListView::clicked, this, [&] (const QModelIndex& index) {
        emit onMenuItemClicked(index.row());
        close();
    });

    auto model = new QStandardItemModel(this);
    listView->setModel(model);

    for (const auto& menu : menus) {
        auto item = new QStandardItem(menu.second);
        QIcon icon;
        icon.addPixmap(menu.first, QIcon::Normal);
        icon.addPixmap(menu.first, QIcon::Selected);
        item->setIcon(icon);
        item->setSizeHint({ 0, 32 });
        model->appendRow(item);
    }

    boxLayout->addWidget(listView);

    auto graphicsEffect = new QGraphicsDropShadowEffect(this);
    graphicsEffect->setOffset(0, 0);    //设置阴影无偏移
    graphicsEffect->setBlurRadius(5);
    graphicsEffect->setColor(0x101010);
    listView->setGraphicsEffect(graphicsEffect);
}
