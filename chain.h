#ifndef CHAIN_H
#define CHAIN_H
#include <qobject.h>
#include <windows.h>
#include <qdebug.h>
#include <qstring.h>
#include <qjsonobject.h>
#include <qeventloop.h>
#include <qtimer.h>
#include <qjsondocument.h>

class Chain : public QObject
{
    Q_OBJECT
public:
    Chain(HANDLE handle, int fakeThreadStack);
    int baseAddress = 0x0;
    bool makeChain();
private:
    HANDLE handle;
    int fakeThreadStack;
    int threadStack = 0x0;
    int offset0 = -0x0;
    static const int offset1 = 0x134;
    static const int offset2 = 0x60;
    static const int offset3 = 0x30;
    static const int citOffset0 = -0x3D0;
    static const int citOffset1 = 0x1918;
    bool isValid();
    QJsonObject fromMemoryValues();
    QJsonObject objWriteToMemory;
    void writeToMemory();
signals:
    void setValues(QJsonObject jsonObject);
public slots:
    void init();
    void getValues(QJsonDocument jsonObject);
};

#endif // CHAIN_H
