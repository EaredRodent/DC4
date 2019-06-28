#ifndef CAMERA_H
#define CAMERA_H
#include <windows.h>
#include <qdebug.h>
#include <qstring.h>

class Chain {
public:
    Chain() {}
    bool success = false;
    void makeChain(HANDLE handle, PVOID fakeThreadStack);
    int ddsdgs() {
        return 9;
    }
private:
    int threadStack = 0x0;
    int offset1 = 0x0;
    static const int offset0 = -0x3A8;
    static const int offset2 = 0x60;
    static const int offset3 = 0x30;
};

class Camera
{
public:
    Camera() {}
    static const int zoom = 0x50;
};

#endif // CAMERA_H
