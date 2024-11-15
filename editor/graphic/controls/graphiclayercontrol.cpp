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
    layer<ActiveNodeLayer>(GraphicLayerType::Layer_Active_Node)->activeNode = nullptr;
    layer<ActiveLinkLineLayer>(GraphicLayerType::Layer_Active_Link)->activeLinkLineList.clear();
    d->graphicObjects.clear();
    graphLayerReload();
}

void GraphicLayerControl::setActiveNode(const GraphicObject* activeNode) {
    layer<ActiveNodeLayer>(GraphicLayerType::Layer_Active_Node)->activeNode = const_cast<GraphicObject*>(activeNode);
    reloadLayer(GraphicLayerType::Layer_Active_Node);
    if (activeNode != nullptr) {
        //测试是否有连接线
        for (int i = 0; i < d->graphicObjects.index(); i++) {
            auto linkLine = dynamic_cast<const GraphicLinkLine*>(d->graphicObjects.command(i));
            if (linkLine == nullptr) {
                continue;
            }
            if (linkLine->data->assignRemoved) {
                continue;
            }
            if (linkLine->linkData->linkFromNode == activeNode ||
                linkLine->linkData->linkToNode == activeNode)
            {
                linkLine->linkData->selected = true;
                layer<ActiveLinkLineLayer>(GraphicLayerType::Layer_Active_Link)->activeLinkLineList.insert(const_cast<GraphicLinkLine*>(linkLine));
            }
        }
        reloadLayer(GraphicLayerType::Layer_Active_Link);
    }
}

void GraphicLayerControl::setActiveLinkLine(GraphicLinkLine* activeLinkLine) {
    layer<ActiveLinkLineLayer>(GraphicLayerType::Layer_Active_Link)->activeLinkLineList.insert(activeLinkLine);
    reloadLayer(GraphicLayerType::Layer_Active_Link);
}

void GraphicLayerControl::cancelActiveLinkLine(const GraphicLinkLine* activeLinkLine) {
    activeLinkLine->linkData->selected = false;
    layer<ActiveLinkLineLayer>(GraphicLayerType::Layer_Active_Link)->activeLinkLineList.remove(const_cast<GraphicLinkLine*>(activeLinkLine));
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

void GraphicLayerControl::updateStaticNodes(QUndoStack* nodes, bool layerReload) {
    layer<StaticNodeLayer>(GraphicLayerType::Layer_Static_Node)->graphicEntries = nodes;
    if (layerReload) {
        reloadLayer(GraphicLayerType::Layer_Static_Node);
    }
}

void GraphicLayerControl::updateStaticLinkLines(QUndoStack* linkLines, bool layerReload) {
    layer<StaticLinkLineLayer>(GraphicLayerType::Layer_Static_Link)->graphicEntries = linkLines;
    if (layerReload) {
        reloadLayer(GraphicLayerType::Layer_Static_Link);
    }
}

void GraphicLayerControl::makeAllStaticNodeChanged() const {
    for (int i = 0; i < d->graphicObjects.index(); i++) {
        auto node = dynamic_cast<const GraphicNode*>(d->graphicObjects.command(i));
        if (node == nullptr) {
            continue;
        }
        if (node->data->assignRemoved) {
            continue;
        }
        node->data->isChanged = true;
    }
}
