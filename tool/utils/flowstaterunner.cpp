#include "flowstaterunner.h"

#include <qdebug.h>

FlowStateRunner::FlowStateRunner(QObject *parent)
    : QObject(parent)
{
}

void FlowStateRunner::setName(const QString &name) {
    m_name = name;
    qDebug() << "FlowStateRunner::setName" << m_name;
}

QString FlowStateRunner::name() const {
    return m_name;
}

void FlowStateRunner::setData(int data) {
    m_data = data;
    qDebug() << "FlowStateRunner::setData" << m_data;
}

int FlowStateRunner::data() const {
    return m_data;
}
