#include "channel.h"

Channel::Channel(QObject *parent) : QObject(parent)
{

}

QJsonObject Channel::getJsonObject()
{
    return mJsonObject;
}

void Channel::readValues(QJsonObject jsonObject)
{
    qDebug() << jsonObject;
    mJsonObject =  jsonObject;
}

void Channel::writeValues(QJsonObject jsonObject)
{

}
