#pragma once

#include <qobject.h>

#include "../commonnodestaterender.h"

#include "../../objects/nodes/noderecoverystate.d.h"

class NodeRecoveryStateRender : public CommonNodeStateRender {
public:
    explicit NodeRecoveryStateRender(const QSharedPointer<NodeRecoveryStateData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<NodeRecoveryStateData> d;
};
