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
    Q_INVOKABLE QJsonObject getVal();
    Q_INVOKABLE void setVal(QJsonObject jsonObject);
    Q_PROPERTY(QJsonObject values READ getVal WRITE setVal NOTIFY changedJsonObject)
public:
    explicit Bridge(QObject *parent = nullptr);

private:
    QJsonObject rJsonObject;

signals:
    void changedJsonObject(QJsonObject jsonObject);
    void setValues(QJsonDocument);

public slots:
    void getValues(QJsonObject jsonObject);
};

#endif // BRIDGE_H
