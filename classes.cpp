#include "classes.h"

void Chain::makeChain(HANDLE handle, PVOID fakeThreadStack) {
    PVOID lastResult;
    float check;
    this->success = false;

    for (PVOID fTS = fakeThreadStack; fTS > (fakeThreadStack - 0x1000); fTS -= 4)
    {
        for (PVOID offset1 = 0; offset1 < 0x7ff; offset1 += 4)
        {
            ReadProcessMemory(handle, fTS + Chain::offset0, &lastResult, sizeof(PVOID), 0);
            ReadProcessMemory(handle, lastResult + offset1, &lastResult, sizeof(PVOID), 0);
            ReadProcessMemory(handle, lastResult + Chain::offset2, &lastResult, sizeof(PVOID), 0);
            ReadProcessMemory(handle, lastResult + Chain::offset3, &lastResult, sizeof(PVOID), 0);

            ReadProcessMemory(handle, lastResult + Camera::zoom, &check, sizeof(float), 0);
            if ((12 <= check) && (check <= 25) && (check == ((int) check)))
            {
                ReadProcessMemory(handle, lastResult + Camera::zoom, &check, sizeof(float), 0);
                if (check == 12)
                {
                    ReadProcessMemory(handle, lastResult + Camera::zoom, &check, sizeof(float), 0);
                    if (check == 25)
                    {
                        this->threadStack = fTS;
                        this->offset1 = offset1;
                        this->success = true;
                        break;
                    }
                }
            }
        }
        if (this->success)
        {
            break;
        }
        qDebug() << (QString) "Chain::makeChain: " + QString::number(100 - (((fTS - (fakeThreadStack - 0x1000)) * 100) / 0x1000)) + "%";
    }
}

int Chain::ddsdgs() {
    return 9;
}
