#include "lolka.h"

Lolka::Lolka(QObject *parent) : QObject(parent)
{

}

void Lolka::takeValues(QJsonObject jsonObject)
{
    qDebug() << jsonObject;
}
