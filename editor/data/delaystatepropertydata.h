#pragma once

#include "datakey.h"

using namespace QDataUtil;

struct DelayStatePropertyData : DataDumpInterface {
    DATA_KEY(int, delayMs); //延迟时间

    DelayStatePropertyData() {
        delayMs = 1000;
    }

    QList<DataReadInterface *> prop() override {
        return { &delayMs };
    }
};