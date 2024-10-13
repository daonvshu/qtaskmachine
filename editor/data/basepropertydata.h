#pragma once

#include "datakey.h"

using namespace QDataUtil;

struct PropertyBindData : DataDumpInterface {

    DATA_KEY(bool, callOnEntered); //进入还是退出时调用
    DATA_KEY(QString, key); //属性key
    DATA_KEY(QString, value); //属性值
    DATA_KEY(QString, valueType); //属性值类型

    QList<DataReadInterface *> prop() override {
        return { &callOnEntered, &key, &value, &valueType };
    }

    PropertyBindData() {
        callOnEntered = true;
        valueType = "int";
    }
};

struct BasePropertyData : DataDumpInterface {
    DATA_KEY(QString, nodeName);   //节点名称

    DATA_KEY(QString, funcEnter);  //进入状态回调函数
    DATA_KEY(QString, funcExit);   //退出状态回调函数

    DATA_KEY(QList<PropertyBindData>, properties); //属性绑定

    QList<DataReadInterface *> prop() override {
        return { &nodeName, &funcEnter, &funcExit, &properties };
    }

    QStringList propertyBindDisplayStrings() const {
        QStringList displayStrings;
        for (auto& prop : properties()) {
            QString str;
            if (prop.valueType() == "string") {
                str = QString("%1 = \"%2\"");
            } else {
                str = QString("%1 = %2");
            }
            displayStrings << str.arg(prop.key(), prop.value());
        }
        return displayStrings;
    }
};