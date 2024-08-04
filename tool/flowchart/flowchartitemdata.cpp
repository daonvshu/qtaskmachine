#include "flowchartitemdata.h"

FlowChartMimeData::FlowChartMimeData(FlowChartNodeType nodeType)  {
    setCustomData(nodeType);
}

FlowChartNodeType FlowChartMimeData::unpack(const QMimeData *mimeData) {
    auto data = mimeData->data(mimeType());
    QDataStream stream(&data, QIODevice::ReadOnly);
    int nodeType;
    stream >> nodeType;
    return FlowChartNodeType(nodeType);
}

void FlowChartMimeData::setCustomData(FlowChartNodeType nodeType) {
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out << (int)nodeType;
    setData(mimeType(), byteArray);
}