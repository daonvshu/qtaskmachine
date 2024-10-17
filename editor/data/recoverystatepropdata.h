#pragma once

#include "datakey.h"

using namespace QDataUtil;

struct RecoveryStatePropertyData : DataDumpInterface {
    DATA_KEY(bool, nested); //嵌套历史状态

    QList<DataReadInterface *> prop() override {
        return { &nested };
    }
};