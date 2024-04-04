#include "multieventstate.h"

MultiEventState::MultiEventState(QState *parent)
    : LinearState(parent)
{
}

void MultiEventState::onEntry(QEvent *event) {
    LinearState::onEntry(event);

    clearTransitions();

    Q_ASSERT(selectState.targetStates.size() >= signalTransitions.size());
    for (int i = 0; i < signalTransitions.size(); i++) {
        auto signalTransition = signalTransitions.at(i);
        //目标信号触发转移
        signalTransition->setTargetState(selectState.targetStates.at(i));
        signalTransition->setSignalDataHandler([&](const QVariantList &data) {
            return testFinishBySignalData(data);
        });
        addTransition(signalTransition);
    }

    //子状态转移
    if (hasChild) {
        if (selectState.targetStates.size() > signalTransitions.size()) { //信号切换列表的下一个为目标状态转移
            addTransition(this, &QState::finished, selectState.targetStates.at(signalTransitions.size()));
        }
    }
}

bool MultiEventState::testFinishBySignalData(const QVariantList &data) {
    return true;
}
