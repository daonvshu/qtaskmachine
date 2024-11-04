#include "graphiclayercontrol.h"

#include "../layer/orthogonalgridlayer.h"
#include "../layer/activenodelayer.h"
#include "../layer/activelinklinelayer.h"
#include "../layer/staticnodelayer.h"
#include "../layer/staticlinklinelayer.h"

#include <qelapsedtimer.h>
#include <qdebug.h>

GraphicLayerControl::GraphicLayerControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent)
    : GraphicControl(data, parent)
{
    layers << qMakePair(GraphicLayerType::Layer_Grid, new OrthogonalGridLayer(this));
    layers << qMakePair(GraphicLayerType::Layer_Static_Link, new StaticLinkLineLayer(this));
    layers << qMakePair(GraphicLayerType::Layer_Static_Node, new StaticNodeLayer(this));
    layers << qMakePair(GraphicLayerType::Layer_Active_Node, new ActiveNodeLayer(this));
    layers << qMakePair(GraphicLayerType::Layer_Active_Link, new ActiveLinkLineLayer(this));
    graphLayerReload();
}

void GraphicLayerControl::graphLayerReload() {
    //重置缓存
    graphicResetOption = GraphicLayerType::Layer_All;
}

void GraphicLayerControl::graphLayerRepaint(QPainter* painter) {
    auto viewSize = d->view->size();
    auto transform = d->getGraphicTransform();
    //QElapsedTimer elapsedTimer;
    //elapsedTimer.start();
    painter->setRenderHint(QPainter::Antialiasing);
    for (auto &i : layers) {
        auto layer = i.second;
        layer->graphicTransform = transform;
        layer->viewSize = viewSize;
        if (graphicResetOption.testFlag(i.first)) {
            layer->reCache();
        }
        layer->reload(painter);
    }
    //qDebug() << "layer repaint time:" << elapsedTimer.elapsed();
    graphicResetOption = GraphicLayerType::Layer_None;
}

void GraphicLayerControl::gridEnable(bool enable) {
    layer<OrthogonalGridLayer>(GraphicLayerType::Layer_Grid)->gridEnabled = enable;
    reloadLayer(GraphicLayerType::Layer_Grid);
}

void GraphicLayerControl::reloadLayer(GraphicLayerTypes layerType) {
    graphicResetOption |= layerType;
}

void GraphicLayerControl::clearAllGraphic() {
    layer<ActiveNodeLayer>(GraphicLayerType::Layer_Active_Node)->activeNode.clear();
    layer<ActiveLinkLineLayer>(GraphicLayerType::Layer_Active_Link)->activeLinkLineList.clear();
    layer<StaticNodeLayer>(GraphicLayerType::Layer_Static_Node)->staticNodeList.clear();
    layer<StaticLinkLineLayer>(GraphicLayerType::Layer_Static_Link)->staticLinkLineList.clear();
    graphLayerReload();
}

void GraphicLayerControl::setActiveNode(const QSharedPointer<GraphicObject> &activeNode) {
    layer<ActiveNodeLayer>(GraphicLayerType::Layer_Active_Node)->activeNode = activeNode;
    reloadLayer(GraphicLayerType::Layer_Active_Node);
    if (activeNode != nullptr) {
        //测试是否有连接线
        const auto& staticLinks = layer<StaticLinkLineLayer>(GraphicLayerType::Layer_Static_Link)->staticLinkLineList;
        for (const auto& linkLine : staticLinks) {
            if (linkLine->linkData->linkFromNode == activeNode ||
                linkLine->linkData->linkToNode == activeNode)
            {
                linkLine->linkData->selected = true;
                layer<ActiveLinkLineLayer>(GraphicLayerType::Layer_Active_Link)->activeLinkLineList.insert(linkLine);
            }
        }
        reloadLayer(GraphicLayerType::Layer_Active_Link);
    }
}

void GraphicLayerControl::setActiveLinkLine(const QSharedPointer<GraphicLinkLine> &activeLinkLine) {
    layer<ActiveLinkLineLayer>(GraphicLayerType::Layer_Active_Link)->activeLinkLineList.insert(activeLinkLine);
    reloadLayer(GraphicLayerType::Layer_Active_Link);
}

void GraphicLayerControl::cancelActiveLinkLine(const QSharedPointer<GraphicLinkLine> &activeLinkLine) {
    activeLinkLine->linkData->selected = false;
    layer<ActiveLinkLineLayer>(GraphicLayerType::Layer_Active_Link)->activeLinkLineList.remove(activeLinkLine);
    reloadLayer(GraphicLayerType::Layer_Active_Link);
}

void GraphicLayerControl::cancelAllActiveLinkLine() {
    auto& activeLinkLines = layer<ActiveLinkLineLayer>(GraphicLayerType::Layer_Active_Link)->activeLinkLineList;
    for (const auto& linkLine : activeLinkLines) {
        linkLine->linkData->selected = false;
    }
    activeLinkLines.clear();
    reloadLayer(GraphicLayerType::Layer_Active_Link);
}

void GraphicLayerControl::updateStaticNodes(const GraphicObjectList &nodes, bool layerReload) {
    layer<StaticNodeLayer>(GraphicLayerType::Layer_Static_Node)->staticNodeList = nodes;
    if (layerReload) {
        reloadLayer(GraphicLayerType::Layer_Static_Node);
    }
}

void GraphicLayerControl::updateStaticLinkLines(const GraphicLinkLineList &linkLines, bool layerReload) {
    layer<StaticLinkLineLayer>(GraphicLayerType::Layer_Static_Link)->staticLinkLineList = linkLines;
    if (layerReload) {
        reloadLayer(GraphicLayerType::Layer_Static_Link);
    }
}

void GraphicLayerControl::makeAllStaticNodeChanged() {
    auto& nodeList = layer<StaticNodeLayer>(GraphicLayerType::Layer_Static_Node)->staticNodeList;
    for (auto& node : nodeList) {
        node->data->isChanged = true;
    }
}
