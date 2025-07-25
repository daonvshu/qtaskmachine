#pragma once

#include "datakey.h"

using namespace QDataUtil;

struct PropertyBindData : DataDumpInterface {
    DATA_KEY(bool, callOnEntered); //进入还是退出时调用
    DATA_KEY(QString, key); //属性key
    DATA_KEY(QString, value); //属性值
    DATA_KEY(QString, valueType); //属性值类型

    QList<DataReadInterface*> prop() override {
        return {&callOnEntered, &key, &value, &valueType};
    }

    PropertyBindData() {
        callOnEntered = true;
        valueType = "int";
    }
};

struct BasePropertyData : DataDumpInterface {
    DATA_KEY(QString, nodeName); //节点名称
    DATA_KEY(QString, nodeId); //节点ID (UUID)

    DATA_KEY(QString, funcEnter); //进入状态回调函数
    DATA_KEY(QString, funcExit); //退出状态回调函数
    DATA_KEY(bool, callNonBlock); //非阻塞执行

    DATA_KEY(bool, printOnEnter); //进入时打印信息
    DATA_KEY(bool, printOnExit); //退出时打印信息

    DATA_KEY(QList<PropertyBindData>, properties); //属性绑定

    BasePropertyData() {
        printOnEnter = true;
        printOnExit = false;
        callNonBlock = true;
    }

    QList<DataReadInterface*> prop() override {
        return {
            &nodeName, &nodeId,
            &funcEnter, &funcExit, &callNonBlock,
            &printOnEnter, &printOnExit,
            &properties
        };
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

    QString searchContent() const {
        auto content = QString("%1 %2 %3").arg(nodeName(), funcEnter(), funcExit());
        for (auto& prop : properties()) {
            content += QString(" %1").arg(prop.key());
        }
        return content.trimmed().toLower();
    }
};
