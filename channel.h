#ifndef CHANNEL_H
#define CHANNEL_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QJsonObject>

class Channel : public QObject
{
    Q_OBJECT
    Q_INVOKABLE QJsonObject getJsonObject();
    Q_PROPERTY(QJsonObject jsonObject READ getJsonObject NOTIFY changeJsonObject)
public:
    explicit Channel(QObject *parent = nullptr);

private:
    QJsonObject mJsonObject;

signals:
    void changeJsonObject() const;

public slots:
    void readValues(QJsonObject jsonObject);
    void writeValues(QJsonObject jsonObject);
};

#endif // CHANNEL_H
