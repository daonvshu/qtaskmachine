#pragma once

#include <qobject.h>

#include "../commonnodestaterender.h"

#include "../../objects/nodes/nodedelaystate.d.h"

class NodeDelayStateRender : public CommonNodeStateRender {
public:
    explicit NodeDelayStateRender(const QSharedPointer<NodeDelayStateData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<NodeDelayStateData> d;
};
