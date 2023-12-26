#pragma once

#include <qobject.h>

enum class StateErrResolveType {
    ResolveRetry,
    ResolveIgnore,
    ResolveQuit,
    NoSet,
};

enum class StateErrReason {
    ReasonTimeout,
    ReasonOperatorFail,
};
Q_DECLARE_METATYPE(StateErrReason)

class StateRecovery : public QObject {
    Q_OBJECT

public:
    explicit StateRecovery(QObject* parent = nullptr);

    StateErrResolveType getResolveType();

    void resetErr();

signals:
    void retry();
    void ignore();
    void quit();

    void stateErrOccur(StateErrReason reason);
    void stateSuccess();

    void fpgaSubStateRetry(); //used for v2

private:
    StateErrResolveType lastStateErrResolveType;
};