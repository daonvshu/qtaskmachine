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
                case FlowChartNodeType::Node_History:
                    break;
                default:
                    FcExecutorItem::drawItemBox(&painter, pixmap.rect(), nodeType);
                    break;
            }

            auto mimeData = new FlowChartMimeData(nodeType);

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

    QRect boxRect(0, 0, 80, 32);
    boxRect.moveCenter(boxDrawRect.center());

    auto nodeType = FlowChartNodeType(index.data(Qt::UserRole + 1).toInt());
    painter->setPen(Qt::white);
    painter->drawText(textRect, Qt::AlignCenter, nodeTypeToString(nodeType));
    painter->setPen(Qt::NoPen);

    switch (nodeType) {
        case FlowChartNodeType::Node_Begin:
        case FlowChartNodeType::Node_End:
            painter->setBrush(QColor(0x13D185));
            painter->drawRoundedRect(boxRect, 4, 4);
            break;
        case FlowChartNodeType::Node_Normal:
            painter->setBrush(QColor(0x598EF3));
            painter->drawRoundedRect(boxRect, 4, 4);
            break;
        case FlowChartNodeType::Node_Delay:
            painter->setBrush(QColor(0x58BCF2));
            painter->drawRoundedRect(boxRect, 4, 4);
            painter->drawPixmap(boxRect.right() - 12, boxRect.bottom() - 12, QPixmap(":/res/time.svg"));
            break;
        case FlowChartNodeType::Node_Event:
            painter->setBrush(QColor(0x58BCF2));
            painter->drawRoundedRect(boxRect, 4, 4);
            painter->drawPixmap(boxRect.right() - 12, boxRect.bottom() - 12, QPixmap(":/res/signal_right.svg"));
            break;
        case FlowChartNodeType::Node_MultiEvent:
            painter->setBrush(QColor(0x58BCF2));
            painter->drawRoundedRect(boxRect, 4, 4);
            painter->drawPixmap(boxRect.right() - 14, boxRect.bottom() - 10, QPixmap(":/res/multi_signal_right.svg"));
            break;
        case FlowChartNodeType::Node_EventCheck:
            painter->setBrush(QColor(0x58BCF2));
            painter->drawRoundedRect(boxRect, 4, 4);
            painter->drawPixmap(boxRect.right() - 12, boxRect.bottom() - 12, QPixmap(":/res/check.svg"));
            break;
        case FlowChartNodeType::Node_Condition: {
            painter->setBrush(QColor(0xF18F01));
            painter->drawRoundedRect(boxRect, 4, 4);
            QPixmap icon(":/res/conditional.svg");
            auto iconRect = icon.rect();
            iconRect.moveCenter(boxRect.center());
            painter->drawPixmap(iconRect, icon);
        }
            break;
        case FlowChartNodeType::Node_History: {
            QPixmap icon(":/res/circle.svg");
            auto iconRect = icon.rect();
            iconRect.moveCenter(boxRect.center());
            painter->drawPixmap(iconRect, icon);
        }
            break;
    }
}

QSize StateTypeListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return { 0, 76 };
}
