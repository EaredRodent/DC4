#ifndef BRIDGE_H
#define BRIDGE_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include "qjsonvalue.h"

class Bridge : public QObject
{
    Q_OBJECT
    Q_INVOKABLE QJsonObject readRead();
    Q_INVOKABLE QJsonObject readWrite();
    Q_INVOKABLE void writeWrite(QJsonObject jsonObject);
    Q_PROPERTY(QJsonObject read READ readRead CONSTANT)
    Q_PROPERTY(QJsonObject write READ readWrite WRITE writeWrite NOTIFY notifyWrite)
public:
    explicit Bridge(QObject *parent = nullptr);

private:
    QJsonObject objectForRead;
    QJsonObject objectForMemoryWrite;

signals:
    void notifyWrite(QJsonObject jsonObject);
    void setValues(QJsonDocument);

public slots:
    void getValues(QJsonObject jsonObject);
};

#endif // BRIDGE_H
