#include "bridge.h"

Bridge::Bridge(QObject *parent) : QObject(parent)
{

}

QJsonObject Bridge::getVal()
{
    return rJsonObject;
}

void Bridge::setVal(QJsonObject jsonObject)
{
    emit setValues(*(new QJsonDocument(jsonObject)));
}


void Bridge::getValues(QJsonObject jsonObject)
{
//    qDebug() << jsonObject;
    rJsonObject =  jsonObject;
}
