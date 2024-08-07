#include "statetypelistview.h"
#include "flowchart/flowchartitemdata.h"

#include "flowchart/elements/fcexecutoritem.h"
#include "flowchart/elements/fcbranchitem.h"

#include <qstandarditemmodel.h>
#include <qpainter.h>
#include <qdrag.h>

StateTypeListView::StateTypeListView(QWidget *parent)
    : QListView(parent)
{
    auto model = new QStandardItemModel(this);
    QListView::setModel(model);

    setItemDelegate(new StateTypeListItemDelegate(this));

    for (int i = 0; i <= (int)FlowChartNodeType::Node_History; i++) {
        auto item = new QStandardItem;
        item->setData(i, Qt::UserRole + 1);
        model->appendRow(item);
    }

    setResizeMode(QListView::Adjust);
}

void StateTypeListView::startDrag(Qt::DropActions supportedActions) {
    if (supportedActions & Qt::MoveAction) {
        auto items = QListView::selectedIndexes();
        if (items.isEmpty()) {
            return;
        }
        auto index = items.first();
        if (index.isValid()) {
            auto nodeType = FlowChartNodeType(index.data(Qt::UserRole + 1).toInt());

            QPixmap pixmap(60, 32 + 10);
            pixmap.fill(Qt::transparent);

            QPainter painter(&pixmap);
            switch (nodeType) {
                case FlowChartNodeType::Node_Condition:
                    FcConditionalItem::drawItemBox(&painter, pixmap.rect());
                    break;
                default:
                    FcExecutorItem::drawItemBox(&painter, pixmap.rect(), nodeType);
                    break;
            }

            FlowChartItemData itemData(nodeType);
            if (nodeType == FlowChartNodeType::Node_Begin) {
                itemData.text = QStringLiteral("开始");
            } else if (nodeType == FlowChartNodeType::Node_End) {
                itemData.text = QStringLiteral("结束");
            }
            auto mimeData = new FlowChartMimeData(itemData);

            auto drag = new QDrag(this);
            drag->setPixmap(pixmap);
            drag->setMimeData(mimeData);
            drag->setHotSpot(pixmap.rect().center());
            drag->exec(supportedActions);

            return;
        }
    }
    QListView::startDrag(supportedActions);
}

void StateTypeListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing);

    QRect textRect = option.rect;
    textRect.setTop(textRect.bottom() - option.fontMetrics.height() - 2 * 4);

    QRect boxDrawRect = option.rect;
    boxDrawRect.setBottom(textRect.top());
    boxDrawRect.adjust(12, 8, -12, 0);

    QRect boxRect(0, 0, 60, 42);
    boxRect.moveCenter(boxDrawRect.center());

    auto nodeType = FlowChartNodeType(index.data(Qt::UserRole + 1).toInt());
    FcExecutorItem::drawItemBox(painter, boxRect, nodeType, QString());

    painter->setPen(Qt::white);
    painter->drawText(textRect, Qt::AlignCenter, nodeTypeToString(nodeType));
}

QSize StateTypeListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return { 0, 76 };
}
