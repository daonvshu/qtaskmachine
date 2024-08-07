#include "flowchartitemdata.h"

FlowChartMimeData::FlowChartMimeData(const FlowChartItemData& itemData)  {
    setCustomData(itemData);
}

FlowChartItemData FlowChartMimeData::unpack(const QMimeData *mimeData) {
    auto data = mimeData->data(mimeType());
    QDataStream stream(&data, QIODevice::ReadOnly);
    FlowChartItemData itemData;
    stream >> itemData;
    return itemData;
}

void FlowChartMimeData::setCustomData(const FlowChartItemData& itemData) {
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out << itemData;
    setData(mimeType(), byteArray);
}