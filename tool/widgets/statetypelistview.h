#pragma once

#include <qobject.h>
#include <qlistview.h>
#include <qstyleditemdelegate.h>

class StateTypeListView : public QListView {
public:
    explicit StateTypeListView(QWidget *parent = nullptr);

protected:
    void startDrag(Qt::DropActions supportedActions) override;
};

class StateTypeListItemDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};