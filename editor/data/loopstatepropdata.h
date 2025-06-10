#pragma once

#include "datakey.h"

using namespace QDataUtil;

struct LoopStatePropertyData : DataDumpInterface {
    DATA_KEY(QString, total); // 总循环次数（属性名称）
    DATA_KEY(QString, exitFunc); // 退出检查函数

    LoopStatePropertyData() {
        total = "-1";
    }

    QList<DataReadInterface*> prop() override {
        return {
            &total,
            &exitFunc
        };
    }

    QString searchContent() const {
        return (total() + "" + exitFunc()).trimmed().toLower();
    }
};