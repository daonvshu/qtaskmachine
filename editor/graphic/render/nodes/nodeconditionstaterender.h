#pragma once

#include <qobject.h>

#include "../commonnodestaterender.h"

#include "../../objects/nodes/nodeconditionstate.d.h"

class NodeConditionStateRender : public CommonNodeStateRender {
public:
    explicit NodeConditionStateRender(const QSharedPointer<NodeConditionStateData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<NodeConditionStateData> d;
};
