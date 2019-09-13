#ifndef BRIDGE_H
#define BRIDGE_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include "QJsonValue"
#include <QVariantMap>

class Bridge : public QObject
{
    Q_OBJECT

public:
    explicit Bridge(QObject *parent = nullptr);

private:
    QJsonObject objectForRead;
    QJsonObject objectForMemoryWrite;

signals:
    void toJavaScript(QVariantMap);
    void toChain(QVariantMap);

public slots:
    void fromChain(QVariantMap map) {
        emit toJavaScript(map);
    }

    void fromJavaScript(QJsonValue value) {
        emit toChain(value.toObject().toVariantMap());
    }

    void cmdExecute(QString string) {
        system(string.toStdString().c_str());
    }
};

#endif // BRIDGE_H
