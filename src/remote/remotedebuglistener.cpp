#include "remotedebuglistener.h"

#include <qdebug.h>

PROTOCOL_CODEC_USING_NAMESPACE
namespace TaskMachine {
    void RemoteDebugListener::startService(int port) {
        instance().createListener(port);
    }

    RemoteDebugListener::RemoteDebugListener() {
        tcpServer = new QTcpServer(this);
        connect(tcpServer, &QTcpServer::newConnection, this, &RemoteDebugListener::onNewConnection);

        connect(this, &RemoteDebugListener::sendLog, this, &RemoteDebugListener::onSendLog);
        connect(this, &RemoteDebugListener::activeNode, this, &RemoteDebugListener::onActiveNode);
        connect(this, &RemoteDebugListener::flowBegin, this, &RemoteDebugListener::onFlowBegin);
        connect(this, &RemoteDebugListener::flowFinished, this, &RemoteDebugListener::onFlowFinished);

        protocolEngine.frameDeclare("H(7E3E3F)S2CV(CRC16)E(FE)");
        protocolEngine.setVerifyFlags("SC");
        protocolEngine.registerType<GetFlowStateRequest>(this, &RemoteDebugListener::onReceiveStateReadRequest);
        protocolEngine.registerCompressEncoder<SendFlowState, JsonCodec>();
        protocolEngine.registerType<SendNewLog, JsonCodec>();
        protocolEngine.registerType<SendActiveNode, JsonCodec>();
        protocolEngine.registerType<SendFlowBegin, JsonCodec>();
        protocolEngine.registerType<SendFlowFinished, JsonCodec>();
    }

    RemoteDebugListener &RemoteDebugListener::instance() {
        static RemoteDebugListener listener;
        return listener;
    }

    void RemoteDebugListener::createListener(int port) {
        if (port == -1) {
            return;
        }
        if (tcpServer->isListening()) {
            return;
        }
        if (!tcpServer->listen(QHostAddress::LocalHost, port)) {
            qCritical() << "QTaskMachine remote debug listener create failed:" << tcpServer->errorString();
        } else {
            qInfo() << "QTaskMachine remote debug listen on port:" << port;
        }
    }

    void RemoteDebugListener::onNewConnection() {
        auto socket = tcpServer->nextPendingConnection();
        remoteConnections.append(socket);
        connect(socket, &QTcpSocket::readyRead, this, [=] {
            protocolEngine.appendBuffer(socket->readAll());
        });
        connect(socket, &QTcpSocket::disconnected, this, [=] {
            remoteConnections.removeOne(socket);
        });
    }

    void RemoteDebugListener::onSendLog(const QString &flowName, qint64 timestamp, const QString &log, QtMsgType msgType) {
        FlowRunLogInfo logInfo;
        logInfo.log = log;
        logInfo.msgType = msgType;
        logInfo.timestamp = timestamp;
        if (!remoteConnections.isEmpty()) {
            SendNewLog data;
            data.flowName = flowName;
            data.log = logInfo;
            auto encodeFrame = protocolEngine.encode(data);
            for (auto socket: remoteConnections) {
                socket->write(encodeFrame);
            }
        }
        flowStates[flowName].flowName = flowName;
        flowStates[flowName].logs() << logInfo;
    }

    void RemoteDebugListener::onActiveNode(const QString &flowName, qint64 timestamp, const QString &nodeId) {
        if (!remoteConnections.isEmpty()) {
            SendActiveNode data;
            data.flowName = flowName;
            data.timestamp = timestamp;
            data.nodeId = nodeId;
            auto encodeFrame = protocolEngine.encode(data);
            for (auto socket: remoteConnections) {
                socket->write(encodeFrame);
            }
        }
        flowStates[flowName].currentRunId = nodeId;
    }

    void RemoteDebugListener::onFlowBegin(const QString &flowName) {
        if (!remoteConnections.isEmpty()) {
            SendFlowBegin data;
            data.flowName = flowName;
            auto encodeFrame = protocolEngine.encode(data);
            for (auto socket: remoteConnections) {
                socket->write(encodeFrame);
            }
        }
        flowStates.remove(flowName);
    }

    void RemoteDebugListener::onFlowFinished(const QString &flowName) {
        if (!remoteConnections.isEmpty()) {
            SendFlowFinished data;
            data.flowName = flowName;
            auto encodeFrame = protocolEngine.encode(data);
            for (auto socket: remoteConnections) {
                socket->write(encodeFrame);
            }
        }
    }

    void RemoteDebugListener::onReceiveStateReadRequest() {
        SendFlowState data;
        data.flowStates = flowStates.values();
        auto encodeFrame = protocolEngine.encode(data);
        for (auto socket : remoteConnections) {
            socket->write(encodeFrame);
        }
    }
}