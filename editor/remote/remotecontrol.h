#pragma once

#include <qobject.h>


class RemoteControl : public QObject {
public:
    explicit RemoteControl(QObject* parent = nullptr);
};
