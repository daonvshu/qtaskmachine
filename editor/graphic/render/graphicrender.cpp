#include "graphicrender.h"

#include "../objects/graphicnode.d.h"

#include <qmath.h>
#include <qicon.h>
#include <qelapsedtimer.h>

// Qt internal function (qtbase/src/widgets/effects/qpixmapfilter.cpp)
extern void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed);

void GraphicRenderInterface::drawGraphicObject(QPainter *painter, bool isActiveState) {
    auto render = getRender();
    if (render == nullptr) {
        return;
    }
    render->renderPainter = painter;
    render->graphicTransform = graphicTransform;
    render->drawObject(isActiveState);
    delete render;
}

void GraphicRenderInterface::drawObject(bool isActiveState) {
    qFatal("not implement graphic render!");
}

GraphicRenderInterface *GraphicRenderInterface::getRender() {
    qFatal("not implement graphic render!");
    return nullptr;
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
    auto font = renderPainter->font();
    font.setPixelSize(pixelSize);
    return QFontMetrics(font).horizontalAdvance(text);
}

QRectF GraphicRenderInterface::getConnectPointRect(const QRectF &itemRect, bool onLeft) {
    qreal cx = onLeft ? (itemRect.left() + 8) : (itemRect.right() - 8);
    qreal cy = itemRect.center().y();
    return { cx - 4, cy - 4, 8, 8 };
}

void GraphicRenderInterface::drawNodeBody(const QRectF& rect, QSharedPointer<GraphicNodeData>& nodeData) {

    QRectF guiBodyRect = graphicTransform.toGuiPoint(rect);
    const qreal shadowRadius = 6;

    if (nodeData->oldBackgroundGuiSize != guiBodyRect.size()) {
        auto radius = graphicTransform.toGuiDx(9);

        //TODO: draw only the visible region
        QPixmap cacheImage(qRound(guiBodyRect.width()), qRound(guiBodyRect.height()));
        cacheImage.fill(Qt::transparent);
        QPainter cachePainter(&cacheImage);
        cachePainter.setRenderHint(QPainter::Antialiasing);
        cachePainter.setBrush(QColor(0x4A4B53));
        cachePainter.setPen(Qt::NoPen);
        cachePainter.drawRoundedRect(QRectF(0, 0, guiBodyRect.width(), guiBodyRect.height()), radius, radius);
        cachePainter.end();

        //QElapsedTimer elapsedTimer;
        //elapsedTimer.start();
        auto shadowImage = drawShadow(cacheImage, shadowRadius, 0xA01E1F22, QPointF(1, 1));
        //qDebug() << "crete shadow image used time:" << elapsedTimer.elapsed();

        nodeData->nodeBackgroundCache = QPixmap(shadowImage.size());
        nodeData->nodeBackgroundCache.fill(Qt::transparent);
        QPainter backgroundPainter(&nodeData->nodeBackgroundCache);
        // draw shadow image
        backgroundPainter.drawImage(QPointF(0, 0), shadowImage);
        // draw source image
        backgroundPainter.drawPixmap(QPointF(shadowRadius, shadowRadius), cacheImage);

        nodeData->oldBackgroundGuiSize = guiBodyRect.size();
    }
    // draw cached image
    renderPainter->drawPixmap(guiBodyRect.topLeft() - QPointF(shadowRadius, shadowRadius), nodeData->nodeBackgroundCache);

    if (nodeData->selected) {
        auto radius = graphicTransform.toGuiDx(9);

        renderPainter->save();
        renderPainter->setPen(Qt::white);
        auto offset = graphicTransform.toGuiDx(8);
        renderPainter->drawRoundedRect(guiBodyRect.adjusted(-offset, -offset, offset, offset), radius, radius);
        renderPainter->restore();
    }
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
        }
        return Qt::black;
    };

    auto drawRect = nodeBodyRect.translated(0, 40);
    drawRect = graphicTransform.toGuiPoint(drawRect);

    renderPainter->save();
    auto pen = renderPainter->pen();
    pen.setColor(getColor().darker(150));
    pen.setWidthF(graphicTransform.toGuiDx(1));
    QPointF padding = QPointF(graphicTransform.toGuiDx(2), 0);
    renderPainter->setPen(pen);
    renderPainter->drawLine(drawRect.topLeft() + padding, drawRect.topRight() - padding);
    renderPainter->restore();
}

void GraphicRenderInterface::drawNodeTitle(const QRectF &renderRect, const QString &title, int pixelSize, int padding) {
    auto font = renderPainter->font();
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize)));
    renderPainter->save();
    renderPainter->setFont(font);
    renderPainter->setPen(Qt::white);
    auto titleDrawRect = graphicTransform.toGuiPoint(renderRect.translated(padding, 0));
    renderPainter->drawText(titleDrawRect, Qt::AlignVCenter | Qt::AlignLeft, title);
    renderPainter->restore();
}

void GraphicRenderInterface::drawConnectableItem(const QRectF &renderRect, const QString &title, int pixelSize, const QColor& color, bool alignLeft, bool linkPointActive) {
    // draw connect point
    renderPainter->save();
    renderPainter->setPen(Qt::NoPen);
    renderPainter->setBrush(color);
    auto connectPointRect = getConnectPointRect(renderRect, alignLeft);
    connectPointRect = graphicTransform.toGuiPoint(connectPointRect);
    renderPainter->drawEllipse(connectPointRect);
    renderPainter->restore();
    if (linkPointActive) {
        renderPainter->save();
        auto pen = renderPainter->pen();
        pen.setWidthF(graphicTransform.toGuiDx(1.5));
        pen.setColor(color);
        renderPainter->setPen(pen);
        auto expandR = graphicTransform.toGuiDx(2);
        renderPainter->drawEllipse(connectPointRect.adjusted(-expandR, -expandR, expandR, expandR));
        renderPainter->restore();
    }
    // draw text
    renderPainter->save();
    auto font = renderPainter->font();
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize)));
    renderPainter->setFont(font);
    renderPainter->setPen(Qt::white);
    auto textRect = renderRect.adjusted(alignLeft ? 16 : 0, 0, alignLeft ? 0 : -16, 0);
    textRect = graphicTransform.toGuiPoint(textRect);
    renderPainter->drawText(textRect, Qt::AlignVCenter | (alignLeft ? Qt::AlignLeft : Qt::AlignRight), title);
    renderPainter->restore();

    //renderPainter->setPen(Qt::yellow);
    //renderPainter->drawRect(graphicTransform.toGuiPoint(renderRect));
}

void GraphicRenderInterface::drawPropertyTitle(const QRectF &renderRect, const QString &title, int pixelSize, int padding) {
    auto font = renderPainter->font();
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize)));
    font.setItalic(true);
    renderPainter->save();
    renderPainter->setFont(font);
    renderPainter->setPen(0xAFAFAF);
    auto titleDrawRect = graphicTransform.toGuiPoint(renderRect.translated(padding, 0));
    renderPainter->drawText(titleDrawRect, Qt::AlignVCenter | Qt::AlignLeft, title);
    renderPainter->restore();
}

void GraphicRenderInterface::drawPropertyRow(const QRectF &renderRect, const QString &title, int pixelSize, const QColor &color) {
    // draw type indicator
    renderPainter->save();
    renderPainter->setPen(Qt::NoPen);
    renderPainter->setBrush(color);
    QRectF indicatorRect(renderRect.left() + 4, renderRect.center().y() - 4, 8, 8);
    indicatorRect = graphicTransform.toGuiPoint(indicatorRect);
    renderPainter->drawRect(indicatorRect);
    renderPainter->restore();
    // draw text
    renderPainter->save();
    auto font = renderPainter->font();
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize)));
    renderPainter->setFont(font);
    renderPainter->setPen(Qt::white);
    auto textRect = renderRect.adjusted(16, 0, 0, 0);
    textRect = graphicTransform.toGuiPoint(textRect);
    renderPainter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, title);
    renderPainter->restore();
}

void GraphicRenderInterface::drawIconRow(const QRectF &renderRect, const QString &iconPath, int iconSize, const QString &displayText, int pixelSize, bool alignLeft) {
    // draw icon
    renderPainter->save();
    QRectF iconRect(0, 0, iconSize, iconSize);
    if (alignLeft) {
        iconRect.moveCenter(QPointF(renderRect.left() + 8 + iconRect.width() / 2, renderRect.center().y()));
    } else {
        iconRect.moveCenter(QPointF(renderRect.right() - 8 - iconRect.width() / 2, renderRect.center().y()));
    }
    iconRect = graphicTransform.toGuiPoint(iconRect);
    QIcon icon(iconPath);
    auto pixmap = icon.pixmap(qRound(iconRect.width()), qRound(iconRect.height()));
    renderPainter->drawPixmap(iconRect.toRect(), pixmap);
    renderPainter->restore();
    // draw icon text
    renderPainter->save();
    auto font = renderPainter->font();
    font.setPixelSize(qRound(graphicTransform.toGuiDx(pixelSize)));
    renderPainter->setFont(font);
    renderPainter->setPen(Qt::white);
    auto textRect = renderRect.adjusted(alignLeft ? (iconSize + 12) : 0, 0, alignLeft ? 0 : -(iconSize + 12), 0);
    textRect = graphicTransform.toGuiPoint(textRect);
    renderPainter->drawText(textRect, Qt::AlignVCenter | (alignLeft ? Qt::AlignLeft : Qt::AlignRight), displayText);
    renderPainter->restore();
}
