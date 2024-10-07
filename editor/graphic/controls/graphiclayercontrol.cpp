#include "graphiclayercontrol.h"

#include "../layer/orthogonalgridlayer.h"
#include "../layer/activenodelayer.h"
#include "../layer/activelinklinelayer.h"
#include "../layer/staticnodelayer.h"
#include "../layer/staticlinklinelayer.h"

#include <qdebug.h>

GraphicLayerControl::GraphicLayerControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent)
    : GraphicControl(data, parent)
{
    layers << qMakePair(GraphicLayerType::Layer_Grid, new OrthogonalGridLayer(this));
    layers << qMakePair(GraphicLayerType::Layer_Active_Node, new ActiveNodeLayer(this));
    layers << qMakePair(GraphicLayerType::Layer_Static_Node, new StaticNodeLayer(this));
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
            layer->reCache();
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

void GraphicLayerControl::setActiveNode(const QSharedPointer<GraphicObject> &activeNode) {
    layer<ActiveNodeLayer>(GraphicLayerType::Layer_Active_Node)->activeNode = activeNode;
    reloadLayer(GraphicLayerType::Layer_Active_Node);
}

void GraphicLayerControl::updateStaticNodes(const GraphicObjectList &nodes) {
    layer<StaticNodeLayer>(GraphicLayerType::Layer_Static_Node)->staticNodeList = nodes;
    reloadLayer(GraphicLayerType::Layer_Static_Node);
}
