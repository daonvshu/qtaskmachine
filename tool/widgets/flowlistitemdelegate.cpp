#include "flowlistitemdelegate.h"

#include <qlineedit.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qevent.h>
#include <qdebug.h>

void FlowListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    //背景
    auto drawRect = option.rect.adjusted(0, 4, 0, -4);
    QPainterPath path;
    path.addRoundedRect(drawRect, 3, 3);
    if (option.state.testFlag(QStyle::State_Selected)) {
        painter->fillPath(path, QBrush(0x63593F));
    } else {
        painter->fillPath(path, QBrush(0x43444B));
    }
    //文字
    painter->drawText(drawRect.adjusted(6, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
    //删除图标
    QPixmap removeIcon(":/res/delete.svg");
    drawRect = removeIcon.rect();
    drawRect.moveCenter(QPoint(option.rect.right() - removeIcon.width() / 2 - 20, option.rect.top() + option.rect.height() / 2));
    painter->drawPixmap(drawRect, removeIcon);
}

void FlowListItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if (auto lineEdit = dynamic_cast<QLineEdit*>(editor)) {
        auto text = lineEdit->text();
        emit nameChanged(index, text);
        model->setData(index, text, Qt::DisplayRole);
    }
}

QSize FlowListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return { 0, 42 };
}

bool FlowListItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) {
    if (event->type() == QEvent::MouseButtonPress) {
        auto pos = dynamic_cast<QMouseEvent *>(event)->pos();
        auto removeRect = QRect(option.rect.right() - 40, option.rect.top(), 40, option.rect.height());
        if (removeRect.contains(pos)) {
            emit clickRemove(index.row());
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
