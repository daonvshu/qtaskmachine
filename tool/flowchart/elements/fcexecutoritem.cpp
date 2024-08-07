#include "fcexecutoritem.h"

#include <qpainter.h>
#include <qstyleoption.h>
#include <qapplication.h>
#include <qdebug.h>

FcExecutorItem::FcExecutorItem(const FlowChartItemData &itemData, QGraphicsItem *parent)
    : FlowChartExecutorItem(parent)
    , itemData(itemData)
{
}

void FcExecutorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    drawItemBox(painter, option->exposedRect, itemData.nodeType, itemData.text);
    FlowChartExecutorItem::paint(painter, option, widget);
}

void FcExecutorItem::drawItemBox(QPainter *painter, const QRectF& rect, FlowChartNodeType nodeType, const QString& text) {
    painter->setRenderHint(QPainter::Antialiasing);
    auto drawRect = rect.adjusted(5, 5, -5, -5);
    //背景
    painter->setPen(Qt::NoPen);
    auto boxRect = drawRect.toRect();
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

    //painter->setBrush(QBrush(0x9C27B0));
    //painter->drawRoundedRect(drawRect, 9, 9);
    //文字
    painter->setPen(Qt::white);
    painter->drawText(drawRect, Qt::AlignCenter, text);
}

QSizeF FcExecutorItem::itemSize() const {
    auto fm = qApp->fontMetrics();
    auto textWidth = fm.horizontalAdvance(itemData.text);
    return { qMax(60.0, qreal(textWidth + 18 + 10)), 32 + 10 };
}

bool FcExecutorItem::acceptableConnectLine() {
    if (itemData.nodeType == FlowChartNodeType::Node_Begin) {
        return false;
    }
    return FlowChartExecutorItem::acceptableConnectLine();
}

bool FcExecutorItem::creatableConnectLine() {
    if (itemData.nodeType == FlowChartNodeType::Node_End) {
        return false;
    }
    return FlowChartExecutorItem::creatableConnectLine();
}
