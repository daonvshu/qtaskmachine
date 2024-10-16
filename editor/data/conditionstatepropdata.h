#pragma once

#include "datakey.h"

using namespace QDataUtil;

struct ConditionStatePropertyData : DataDumpInterface {
    DATA_KEY(QList<int>, branchIds); //分支id列表
    DATA_KEY(QString, conditionFunc); //分支检查函数

    ConditionStatePropertyData() {
        branchIds() << -1 << -1;
    }

    QList<DataReadInterface *> prop() override {
        return { &branchIds, &conditionFunc };
    }
};