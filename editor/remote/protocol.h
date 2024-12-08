#pragma once

#include "data/datakey.h"

using namespace QDataUtil;

template<typename T>
struct DataDumpProtocol : DataDumpInterface {
    //json数据解码
    static T fromJson(const QJsonDocument& doc) {
        T data;
        auto obj = doc.object();
        data.DataDumpInterface::fromJson(obj);
        return data;
    }

    //json数据编码
    QJsonDocument toJson() const {
        T tmp = *dynamic_cast<const T*>(this);
        return QJsonDocument(tmp.dumpToJson());
    }
};

struct FlowRunLogInfo : DataDumpInterface {
    DATA_KEY(QString, log);
    DATA_KEY(qint64, timestamp);
    DATA_KEY(int, msgType);

    QList<DataReadInterface *> prop() override {
        return { &log, &timestamp, &msgType };
    }
};

struct FlowStateInfo : DataDumpInterface {
    DATA_KEY(QString, flowName);
    DATA_KEY(QString, currentRunId);
    DATA_KEY(QList<FlowRunLogInfo>, logs);

    QList<DataReadInterface *> prop() override {
        return { &flowName, &currentRunId, &logs };
    }
};

//send
#define GetFlowStateRequest 1

//receive
struct ReceiveFlowState : DataDumpProtocol<ReceiveFlowState> {
    enum {
        Type = 2
    };

    DATA_KEY(QList<FlowStateInfo>, flowStates);

    QList<DataReadInterface *> prop() override {
        return { &flowStates };
    }
};

struct ReceiveNewLog : DataDumpProtocol<ReceiveNewLog> {
    enum {
        Type = 3
    };

    DATA_KEY(QString, flowName);
    DATA_KEY(FlowRunLogInfo, log);

    QList<DataReadInterface *> prop() override {
        return { &flowName, &log };
    }
};

Q_DECLARE_METATYPE(ReceiveNewLog)

struct ReceiveActiveNode : DataDumpProtocol<ReceiveActiveNode> {
    enum {
        Type = 4
    };

    DATA_KEY(QString, flowName);
    DATA_KEY(qint64, timestamp);
    DATA_KEY(QString, nodeId);

    QList<DataReadInterface *> prop() override {
        return { &flowName, &timestamp, &nodeId };
    }
};

Q_DECLARE_METATYPE(ReceiveActiveNode)

struct ReceiveFlowBegin : DataDumpProtocol<ReceiveFlowBegin> {
    enum {
        Type = 5
    };

    DATA_KEY(QString, flowName);

    QList<DataReadInterface *> prop() override {
        return { &flowName };
    }
};

struct ReceiveFlowFinished : DataDumpProtocol<ReceiveFlowFinished> {
    enum {
        Type = 6
    };

    DATA_KEY(QString, flowName);

    QList<DataReadInterface *> prop() override {
        return { &flowName };
    }
};
