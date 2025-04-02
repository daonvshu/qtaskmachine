#pragma once

#include "datakey.h"

#include "eventstatepropdata.h"

using namespace QDataUtil;

struct MultiEventStatePropertyData : DataDumpInterface {
    DATA_KEY(QList<EventTriggerFunction>, events); // 触发事件列表

    MultiEventStatePropertyData() {
        events() << EventTriggerFunction() << EventTriggerFunction();
    }

    QList<DataReadInterface *> prop() override {
        return {
            &events
        };
    }

    QString searchContent() const {
        QStringList functions;
        for (auto& event : events()) {
            functions << event.triggerFunc() << event.checkFunc();
        }
        return functions.join(" ").trimmed().toLower();
    }
};