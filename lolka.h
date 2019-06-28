#ifndef LOLKA_H
#define LOLKA_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>

class Lolka : public QObject
{
    Q_OBJECT
public:
    explicit Lolka(QObject *parent = nullptr);

signals:

public slots:
    void takeValues(QJsonObject jsonObject);
};

#endif // LOLKA_H
