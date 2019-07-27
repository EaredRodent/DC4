#include "bridge.h"

Bridge::Bridge(QObject *parent) : QObject(parent)
{

}

QJsonObject Bridge::readRead()
{
//    qDebug() << objectForRead;
    return objectForRead;
}

QJsonObject Bridge::readWrite()
{
    return objectForMemoryWrite;
}

void Bridge::writeWrite(QJsonObject jsonObject)
{
    qDebug() << jsonObject;
    emit setValues(*(new QJsonDocument(jsonObject)));
}

void Bridge::getValues(QJsonObject jsonObject)
{
    objectForRead =  jsonObject;
}
