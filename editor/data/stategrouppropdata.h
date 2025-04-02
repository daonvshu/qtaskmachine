#pragma once

#include "datakey.h"

using namespace QDataUtil;

struct StateGroupPropertyData : DataDumpInterface {
    DATA_KEY(QString, errorTriggerFunc); // 错误触发信号函数

    QList<DataReadInterface *> prop() override {
        return { &errorTriggerFunc };
    }

    QString searchContent() const {
        return errorTriggerFunc().trimmed().toLower();
    }
};