#include "remotedebuglistener.h"

#include <qdebug.h>

void RemoteDebugListener::startService(int port) {
    instance().createListener(port);
}

RemoteDebugListener::RemoteDebugListener() {
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &RemoteDebugListener::onNewConnection);
}

RemoteDebugListener& RemoteDebugListener::instance() {
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

    });
    connect(socket, &QTcpSocket::disconnected, this, [=] {
        remoteConnections.removeOne(socket);
    });
}

