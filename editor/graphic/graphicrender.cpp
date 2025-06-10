#include "graphicrender.h"

#include "graphic/objects/graphicnode.d.h"

#include <qmath.h>
#include <qicon.h>
#include <qelapsedtimer.h>
#include <qapplication.h>
#include <qpainterpath.h>

// Qt internal function (qtbase/src/widgets/effects/qpixmapfilter.cpp)
extern void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed);

void GraphicRenderInterface::drawObject() {
    if (cachePainter) {
        cachePainter->end();
    }
}

QRectF GraphicRenderInterface::getNodeBodyRectFromTopCenter(const QPointF &topCenter, int requiredWidth, int requiredHeight) {
    qreal width = qMax(requiredWidth, 120);
    qreal height = requiredHeight;
    return {
            topCenter.x() - width / 2.0,
            topCenter.y(),
            width,
            height
            };
}

int GraphicRenderInterface::getTextWidthByFont(const QString& text, int pixelSize) const {
    auto font = qApp->font();
    font.setPixelSize(pixelSize);
    return QFontMetrics(font).horizontalAdvance(text);
}

QRectF GraphicRenderInterface::getConnectPointRect(const QRectF &itemRect, bool onLeft) {
    qreal cx = onLeft ? (itemRect.left() + 8) : (itemRect.right() - 8);
    qreal cy = itemRect.center().y();
    return { cx - 4, cy - 4, 8, 8 };
}

void GraphicRenderInterface::drawNodeBody(const QRectF& rect) {

    const qreal shadowRadius = 6;
    auto radius = graphicTransform.toGuiDx(9);

    QPixmap bodyImage(qRound(rect.width()), qRound(rect.height()));
    {
        bodyImage.fill(Qt::transparent);
        QPainter bodyPainter(&bodyImage);
        bodyPainter.setRenderHint(QPainter::Antialiasing);
        bodyPainter.setBrush(QColor(0x4A4B53));
        bodyPainter.setPen(Qt::NoPen);
        bodyPainter.drawRoundedRect(QRectF(0, 0, rect.width(), rect.height()), radius, radius);
        bodyPainter.end();
    }

    //QElapsedTimer elapsedTimer;
    //elapsedTimer.start();
    auto shadowImage = drawShadow(bodyImage, shadowRadius, 0xA01E1F22, QPointF(1, 1));
    //qDebug() << "crete shadow image used time:" << elapsedTimer.elapsed();

    // draw shadow image
    cachePainter->drawImage(QPointF(-shadowRadius, -shadowRadius), shadowImage);
    // draw source image
    cachePainter->drawPixmap(QPointF(0, 0), bodyImage);
}

void GraphicRenderInterface::drawNodeRunningState(const QRectF& rect, int titleHeight) {
    auto radius = graphicTransform.toGuiDx(9);
    cachePainter->save();
    cachePainter->setPen(0x0E9E45);
    cachePainter->drawRoundedRect(QRectF(0, 0, rect.width(), rect.height()), radius, radius);
    cachePainter->setBrush(QColor(0x0E9E45));
    QRectF titleRect(0, 0, rect.width(), graphicTransform.toGuiDx(titleHeight));
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(titleRect, radius, radius);
    path.addRect(0, titleRect.height() / 2, titleRect.width(), titleRect.height() / 2);
    cachePainter->drawPath(path);
    cachePainter->restore();
}

// qtbase/src/widgets/effects/qpixmapfilter.cpp: line 1317
QImage GraphicRenderInterface::drawShadow(const QPixmap &shadowObjectPixCache, qreal blurRadius, const QColor &color, const QPointF &offset) {

    // temp render object
    QImage tmp(shadowObjectPixCache.size() + QSize(qCeil(blurRadius * 2), qCeil(blurRadius * 2)), QImage::Format_ARGB32_Premultiplied);
    tmp.setDevicePixelRatio(shadowObjectPixCache.devicePixelRatioF());
    tmp.fill(0);
    QPainter tmpPainter(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawPixmap(QPointF(blurRadius, blurRadius) + offset, shadowObjectPixCache);
    tmpPainter.end();

    // blur the alpha channel
    QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
    blurred.setDevicePixelRatio(shadowObjectPixCache.devicePixelRatioF());
    blurred.fill(0);
    QPainter blurPainter(&blurred);
    qt_blurImage(&blurPainter, tmp, blurRadius, false, true, 0);
    blurPainter.end();

    tmp = std::move(blurred);

    // blacken the image...
    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    tmpPainter.fillRect(tmp.rect(), color);
    tmpPainter.end();

    return tmp;
}

void GraphicRenderInterface::drawNodeSplitLine(const QRectF &nodeBodyRect, GraphicObjectType objectType) {
    auto getColor = [&] () -> QColor {
        switch (objectType) {
            case GraphicObjectType::Node_Begin_State:
                return 0x13D185;
            case GraphicObjectType::Node_End_State:
                return 0x009E9E;
            case GraphicObjectType::Node_Normal_State:
                return 0x598EF3;
            case GraphicObjectType::Node_Delay_State:
                return 0x58BCF2;
            case GraphicObjectType::Node_Event_State:
                return 0x58BCF2;
            case GraphicObjectType::Node_MultiEvent_State:
                return 0x58BCF2;
            case GraphicObjectType::Node_Condition_State:
                return 0xF18F01;
            case GraphicObjectType::Node_State_Group:
                return 0xD165A2;
            case GraphicObjectType::Node_Recovery_State:
                return Qt::red;
            case GraphicObjectType::Node_Loop_State:
                return 0xD1AB00;
            default:
                break;
        }
        return Qt::black;
    };

    auto drawRect = nodeBodyRect.translated(0, 40);
    drawRect = graphicTransform.toGuiPoint(drawRect);
    
    cachePainter->save();
    auto pen = cachePainter->pen();
    pen.setColor(getColor().darker(150));
    pen.setWidthF(graphicTransform.toGuiDx(1));
    QPointF padding = QPointF(graphicTransform.toGuiDx(2), 0);
    cachePainter->setPen(pen);
    cachePainter->drawLine(drawRect.topLeft() + padding, drawRect.topRight() - padding);
    cachePainter->restore();
}

void GraphicRenderInterface::drawNodeTitle(const QRectF &renderRect, const QString &title, int pixelSize, int padding) {
    auto font = cachePainter->font();
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize)));
    cachePainter->save();
    cachePainter->setFont(font);
    cachePainter->setPen(Qt::white);
    auto titleDrawRect = graphicTransform.toGuiPoint(renderRect.translated(padding, 0));
    cachePainter->drawText(titleDrawRect, Qt::AlignVCenter | Qt::AlignLeft, title);
    cachePainter->restore();
}

void GraphicRenderInterface::drawConnectableItem(const QRectF &renderRect, const QString &title, int pixelSize, const QColor& color, bool alignLeft) {
    // draw connect point
    cachePainter->save();
    cachePainter->setPen(Qt::NoPen);
    cachePainter->setBrush(color);
    auto connectPointRect = getConnectPointRect(renderRect, alignLeft);
    connectPointRect = graphicTransform.toGuiPoint(connectPointRect);
    cachePainter->drawEllipse(connectPointRect);
    cachePainter->restore();

    // draw text
    cachePainter->save();
    auto font = cachePainter->font();
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize)));
    cachePainter->setFont(font);
    cachePainter->setPen(Qt::white);
    auto textRect = renderRect.adjusted(alignLeft ? 16 : 0, 0, alignLeft ? 0 : -16, 0);
    textRect = graphicTransform.toGuiPoint(textRect);
    cachePainter->drawText(textRect, Qt::AlignVCenter | (alignLeft ? Qt::AlignLeft : Qt::AlignRight), title);
    cachePainter->restore();

    //cachePainter->setPen(Qt::yellow);
    //cachePainter->drawRect(graphicTransform.toGuiPoint(renderRect));
}

void GraphicRenderInterface::drawDoubleRowConnectableItem(const QRectF &renderRect, const QString &title,
                                                          const QString &subTitle, int pixelSize, const QColor &color,
                                                          bool alignLeft) {
    if (subTitle.isEmpty()) {
        drawConnectableItem(renderRect, title, pixelSize, color, alignLeft);
        return;
    }
    // draw connect point
    cachePainter->save();
    cachePainter->setPen(Qt::NoPen);
    cachePainter->setBrush(color);
    auto connectPointRect = getConnectPointRect(renderRect, alignLeft);
    connectPointRect = graphicTransform.toGuiPoint(connectPointRect);
    cachePainter->drawEllipse(connectPointRect);
    cachePainter->restore();

    // draw text
    cachePainter->save();
    auto font = cachePainter->font();
    // main text
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize - 1)));
    cachePainter->setFont(font);
    cachePainter->setPen(Qt::white);
    auto textRect = renderRect.adjusted(alignLeft ? 16 : 2, 0, alignLeft ? 0 : -16, 0);
    textRect = graphicTransform.toGuiPoint(textRect);
    cachePainter->drawText(textRect, Qt::AlignTop | (alignLeft ? Qt::AlignLeft : Qt::AlignRight), title);
    // sub text
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize - 3)));
    cachePainter->setFont(font);
    cachePainter->setPen(Qt::gray);
    textRect = renderRect.adjusted(alignLeft ? 16 : 0, 0, alignLeft ? 0 : -16, -2);
    textRect = graphicTransform.toGuiPoint(textRect);
    cachePainter->drawText(textRect, Qt::AlignBottom | (alignLeft ? Qt::AlignLeft : Qt::AlignRight), subTitle);

    cachePainter->restore();
}

void GraphicRenderInterface::drawPropertyTitle(const QRectF &renderRect, const QString &title, int pixelSize, int padding) {
    auto font = cachePainter->font();
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize)));
    font.setItalic(true);
    cachePainter->save();
    cachePainter->setFont(font);
    cachePainter->setPen(0xAFAFAF);
    auto titleDrawRect = graphicTransform.toGuiPoint(renderRect.translated(padding, 0));
    cachePainter->drawText(titleDrawRect, Qt::AlignVCenter | Qt::AlignLeft, title);
    cachePainter->restore();
}

void GraphicRenderInterface::drawPropertyRow(const QRectF &renderRect, const QString &title, int pixelSize, const QColor &color, bool alignLeft) {
    // draw type indicator
    cachePainter->save();
    cachePainter->setPen(Qt::NoPen);
    cachePainter->setBrush(color);
    QRectF indicatorRect;
    if (alignLeft) {
        indicatorRect = QRectF(renderRect.left() + 4, renderRect.center().y() - 4, 8, 8);
    } else {
        indicatorRect = QRectF(renderRect.right() - 12, renderRect.center().y() - 4, 8, 8);
    }
    indicatorRect = graphicTransform.toGuiPoint(indicatorRect);
    cachePainter->drawRect(indicatorRect);
    cachePainter->restore();
    // draw text
    cachePainter->save();
    auto font = cachePainter->font();
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize)));
    cachePainter->setFont(font);
    cachePainter->setPen(Qt::white);
    auto textRect = renderRect.adjusted(alignLeft ? 16 : 0, 0, alignLeft ? 0 : -16, 0);
    textRect = graphicTransform.toGuiPoint(textRect);
    cachePainter->drawText(textRect, Qt::AlignVCenter | (alignLeft ? Qt::AlignLeft : Qt::AlignRight), title);
    cachePainter->restore();
}

void GraphicRenderInterface::drawIconRow(const QRectF &renderRect, const QString &iconPath, int iconSize, const QString &displayText, int pixelSize, bool alignLeft) {
    // draw icon
    cachePainter->save();
    QRectF iconRect(0, 0, iconSize, iconSize);
    if (alignLeft) {
        iconRect.moveCenter(QPointF(renderRect.left() + 8 + iconRect.width() / 2, renderRect.center().y()));
    } else {
        iconRect.moveCenter(QPointF(renderRect.right() - 8 - iconRect.width() / 2, renderRect.center().y()));
    }
    iconRect = graphicTransform.toGuiPoint(iconRect);
    QIcon icon(iconPath);
    auto pixmap = icon.pixmap(qRound(iconRect.width()), qRound(iconRect.height()));
    cachePainter->drawPixmap(iconRect.toRect(), pixmap);
    cachePainter->restore();
    // draw icon text
    cachePainter->save();
    auto font = cachePainter->font();
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize)));
    cachePainter->setFont(font);
    cachePainter->setPen(Qt::white);
    auto textRect = renderRect.adjusted(alignLeft ? (iconSize + 12) : 0, 0, alignLeft ? 0 : -(iconSize + 12), 0);
    textRect = graphicTransform.toGuiPoint(textRect);
    cachePainter->drawText(textRect, Qt::AlignVCenter | (alignLeft ? Qt::AlignLeft : Qt::AlignRight), displayText);
    cachePainter->restore();
}
