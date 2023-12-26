#include "staterecovery.h"

StateRecovery::StateRecovery(QObject *parent)
    : QObject(parent)
    , lastStateErrResolveType(StateErrResolveType::NoSet)
{
    qRegisterMetaType<StateErrReason>("StateErrReason");

    connect(this, &StateRecovery::retry, this, [&] {
        lastStateErrResolveType = StateErrResolveType::ResolveRetry;
    });

    connect(this, &StateRecovery::ignore, this, [&] {
        lastStateErrResolveType = StateErrResolveType::ResolveIgnore;
    });

    connect(this, &StateRecovery::quit, this, [&] {
        lastStateErrResolveType = StateErrResolveType::ResolveQuit;
    });
}

StateErrResolveType StateRecovery::getResolveType() {
    return lastStateErrResolveType;
}

void StateRecovery::resetErr() {
    lastStateErrResolveType = StateErrResolveType::NoSet;
}
