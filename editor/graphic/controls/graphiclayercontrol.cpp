#include "graphiclayercontrol.h"

#include "../layer/orthogonalgridlayer.h"

#include <qdebug.h>

GraphicLayerControl::GraphicLayerControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent)
    : GraphicControl(data, parent)
{
    layers << qMakePair(GraphicLayerType::Layer_Grid, new OrthogonalGridLayer(this));
    graphLayerReload();
}

void GraphicLayerControl::graphLayerReload() {
    //重置缓存
    graphicResetOption = GraphicLayerType::Layer_All;
}

void GraphicLayerControl::graphLayerRepaint(QPainter* painter) {
    auto viewSize = d->view->size();
    auto transform = d->getGraphicTransform();
    for (auto &i : layers) {
        auto layer = i.second;
        layer->graphicTransform = transform;
        layer->sizeAdjust(viewSize);
        if (graphicResetOption.testFlag(i.first)) {
            layer->recache();
        }
        painter->drawPixmap(0, 0, layer->layerCache);
    }
    graphicResetOption = GraphicLayerType::Layer_None;
}

void GraphicLayerControl::hoverMoving(const QPoint &mousePoint) {
    //auto realPoint = d->getGraphicTransform().toRealPoint(QPointF(mousePoint));
}

void GraphicLayerControl::gridEnable(bool enable) {
    layer<OrthogonalGridLayer>(GraphicLayerType::Layer_Grid)->gridEnabled = enable;
    reloadLayer(GraphicLayerType::Layer_Grid);
}

void GraphicLayerControl::reloadLayer(GraphicLayerType layerType) {
    graphicResetOption |= layerType;
}

void GraphicLayerControl::clearAllGraphic() {

}
