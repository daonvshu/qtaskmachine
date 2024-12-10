#pragma once

#include <qobject.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>

#include "protocol.h"

#include <protocolcodecengine.h>

class TaskMachineRunner;
namespace TaskMachine {
    class RemoteDebugListener : public QObject {
        Q_OBJECT

    public:
        static void startService(int port);

    signals:
        void sendLog(const QString &flowName, qint64 timestamp, const QString &log, int msgType);
        void activeNode(const QString &flowName, qint64 timestamp, const QString &nodeId);
        void flowBegin(const QString &flowName);
        void flowFinished(const QString &flowName);

    private:
        explicit RemoteDebugListener();

        static RemoteDebugListener &instance();

        void createListener(int port);

        void onNewConnection();

    private slots:
        void onSendLog(const QString &flowName, qint64 timestamp, const QString &log, int msgType);
        void onActiveNode(const QString &flowName, qint64 timestamp, const QString &nodeId);
        void onFlowBegin(const QString &flowName);
        void onFlowFinished(const QString &flowName);
        void onReceiveStateReadRequest();

    private:
        QTcpServer *tcpServer;
        QList<QTcpSocket *> remoteConnections;
        protocol_codec::ProtocolCodecEngine protocolEngine;
        QHash<QString, FlowStateInfo> flowStates;

        friend class ::TaskMachineRunner;
    };
}
