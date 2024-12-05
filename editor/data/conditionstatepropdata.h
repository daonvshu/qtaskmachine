#pragma once

#include "datakey.h"

using namespace QDataUtil;

struct ConditionStatePropertyData : DataDumpInterface {
    DATA_KEY(QList<int>, branchIds); //分支id列表
    DATA_KEY(QStringList, branchNames); //分支名称列表
    DATA_KEY(QString, conditionFunc); //分支检查函数

    ConditionStatePropertyData() {
        branchIds() << 0 << 1;
    }

    QList<DataReadInterface *> prop() override {
        return { &branchIds, &branchNames, &conditionFunc };
    }
};