#ifndef CHAIN_H
#define CHAIN_H
#include <QObject>
#include <QDebug>
#include <QString>
#include <QJsonObject>
#include <QEventLoop>
#include <QTimer>
#include <QJsonDocument>
#include <QVariantMap>
#include <QRegExp>

#include <windows.h>

#include "system.h"

class Chain : public QObject
{
    Q_OBJECT
public:
    Chain(QString version);
    int baseAddress = 0x0;
    bool makeChain();
private:
    DWORD processId = 0x0;
    HANDLE handle = 0x0;
    int fakeThreadStack = 0x0;
    int threadStack = 0x0;

    int offset0 = -0x0;
    static const int offset1 = 0x8;
    static const int offset2 = 0x4E8;
    static const int offset3 = 0x60;
    static const int offset4 = 0x30;

    static const int citOffset0 = -0x3D0;
    static const int citOffset1 = 0x1918;

    bool isValid();
    QVariantMap fromMemoryValues();
    QVariantMap objWriteToMemory;
    void writeToMemory();
    QString getCityName();


    QString version;
signals:
    void toBridge(QVariantMap jsonObject);
public slots:
    void init();
    void fromBridge(QVariantMap map);
};

#endif // CHAIN_H
