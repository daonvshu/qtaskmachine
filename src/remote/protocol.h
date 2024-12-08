#pragma once

#include "stateutils/datakey.h"

namespace TaskMachine {

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
        T_DATA_KEY(QString, log);
        T_DATA_KEY(qint64, timestamp);
        T_DATA_KEY(int, msgType);

        QList<DataReadInterface *> prop() override {
            return { &log, &timestamp, &msgType };
        }
    };

    struct FlowStateInfo : DataDumpInterface {
        T_DATA_KEY(QString, flowName);
        T_DATA_KEY(QString, currentRunId);
        T_DATA_KEY(QList<FlowRunLogInfo>, logs);

        QList<DataReadInterface *> prop() override {
            return { &flowName, &currentRunId, &logs };
        }
    };

    //receive
    #define GetFlowStateRequest 1

    //send
    struct SendFlowState : DataDumpProtocol<SendFlowState> {
        enum {
            Type = 2
        };

        T_DATA_KEY(QList<FlowStateInfo>, flowStates);

        QList<DataReadInterface *> prop() override {
            return { &flowStates };
        }
    };

    struct SendNewLog : DataDumpProtocol<SendNewLog> {
        enum {
            Type = 3
        };

        T_DATA_KEY(QString, flowName);
        T_DATA_KEY(FlowRunLogInfo, log);

        QList<DataReadInterface *> prop() override {
            return { &flowName, &log };
        }
    };

    struct SendActiveNode : DataDumpProtocol<SendActiveNode> {
        enum {
            Type = 4
        };

        T_DATA_KEY(QString, flowName);
        T_DATA_KEY(qint64, timestamp);
        T_DATA_KEY(QString, nodeId);

        QList<DataReadInterface *> prop() override {
            return { &flowName, &timestamp, &nodeId };
        }
    };

    struct SendFlowBegin : DataDumpProtocol<SendFlowBegin> {
        enum {
            Type = 5
        };

        T_DATA_KEY(QString, flowName);

        QList<DataReadInterface *> prop() override {
            return { &flowName };
        }
    };

    struct SendFlowFinished : DataDumpProtocol<SendFlowFinished> {
        enum {
            Type = 6
        };

        T_DATA_KEY(QString, flowName);

        QList<DataReadInterface *> prop() override {
            return { &flowName };
        }
    };
}