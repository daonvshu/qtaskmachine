#include "graphicobjcreatecontrol.h"

#include "../objects/nodes/nodenormalstate.h"

#include "graphiclayercontrol.h"

GraphicObjCreateControl::GraphicObjCreateControl(const QSharedPointer<GraphicControlSharedData> &data, QObject *parent)
    : GraphicControl(data, parent)
{
}

void GraphicObjCreateControl::addObject(GraphicObjectType type, const QPoint& mousePoint) {
    switch (type) {
        case GraphicObjectType::Node_Begin_State:
            break;
        case GraphicObjectType::Node_End_State:
            break;
        case GraphicObjectType::Node_Normal_State:
            activeObject = NodeNormalState::create();
            break;
        case GraphicObjectType::Node_Delay_State:
            break;
        case GraphicObjectType::Node_Event_State:
            break;
        case GraphicObjectType::Node_MultiEvent_State:
            break;
        case GraphicObjectType::Node_Condition_State:
            break;
        case GraphicObjectType::Node_State_Group:
            break;
        case GraphicObjectType::Node_Recovery_State:
            break;
        default:
            break;
    }
    if (activeObject) {
        activeObject->data->renderPosition = d->getGraphicTransform().toRealPoint(mousePoint);
        activeObject->data->selected = true;
    }
    d->getControl<GraphicLayerControl>()->setActiveNode(activeObject);
    d->view->repaint();
}
