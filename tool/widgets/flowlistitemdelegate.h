#pragma once

#include <qobject.h>
#include <qstyleditemdelegate.h>

class FlowListItemDelegate : public QStyledItemDelegate {
Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

signals:
    void nameChanged(const QModelIndex& index, const QString& text) const;
    void clickRemove(int index);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};