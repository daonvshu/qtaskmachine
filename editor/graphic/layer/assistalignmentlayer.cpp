#include "assistalignmentlayer.h"

AssistAlignmentLayer::AssistAlignmentLayer(QObject* parent)
    : GraphicLayer(parent)
{
}

void AssistAlignmentLayer::reload(QPainter* painter) {
    painter->save();

    QPen pen(QColor(0xB18906));
    pen.setWidth(1);
    pen.setStyle(Qt::CustomDashLine);
    pen.setDashPattern({4, 4});

    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing, false);

    for (const auto& line : lines) {
        auto dstLine = graphicTransform.toGuiPoint(line);
        painter->drawLine(dstLine.translated(6, 6));
    }

    painter->restore();
}

void AssistAlignmentLayer::reCache() {

}
