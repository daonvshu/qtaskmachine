#pragma once

#include <qobject.h>


class NodeEventState : public QObject {
public:
    explicit NodeEventState(QObject *parent = nullptr);
};
