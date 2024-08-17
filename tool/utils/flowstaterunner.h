#pragma once

#include <qobject.h>


class FlowStateRunner : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int data READ data WRITE setData NOTIFY dataChanged)

public:
    explicit FlowStateRunner(QObject *parent = nullptr);

    void setName(const QString &name);
    QString name() const;

    void setData(int data);
    int data() const;

signals:
    void nameChanged();
    void dataChanged();

private:
    QString m_name;
    int m_data;
};
