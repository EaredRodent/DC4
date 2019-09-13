#include "chain.h"
#include "camera.h"

Chain::Chain(QString version)
{
    this->version = version;
}

void Chain::init()
{
    QEventLoop *el = new QEventLoop();
    QTimer *tr = new QTimer();
    tr->setSingleShot(true);
    QObject::connect(tr, SIGNAL(timeout()), el, SLOT(quit()));
    QVariantMap output;

    int tryToMake = 0;

    while (1) {
        if(isValid()) {
            tryToMake = 0;
            output = fromMemoryValues();
            output.insert("isValid", true);
            output.insert("version", version);
            output.insert("threadStack", QString::number(threadStack + offset0, 0x10));
            toBridge(output);
            tr->start(500);
        } else {
            tryToMake++;
            output = QVariantMap();
            output.insert("isValid", false);
            output.insert("version", version);
            output.insert("times", tryToMake);
            toBridge(output);
            makeChain();

            tr->start(100);
        }

        el->exec();
    }
}

bool Chain::makeChain() {
    processId = processGetPid("dro_client.exe");
    handle = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);
    fakeThreadStack = getFakeThreadStack(handle, processId);

    threadStack = fakeThreadStack;

    for(int offset0 = 0; offset0 < 0x1000; offset0 += 4) {
        this->offset0 = -offset0;

        if(isValid()) {
//            qDebug() << "TS: " << QString::number(this->threadStack, 0x10) << "OFFSET0: -(" << QString::number(this->offset0 * -1, 0x10) << ")";
            return true;
        }
    }

    return false;
}

bool Chain::isValid()
{
    int baseAddress;

    ReadProcessMemory(handle, (int *)(threadStack + offset0), &baseAddress, sizeof(int), 0);
    ReadProcessMemory(handle, (int *)(baseAddress + Chain::offset1), &baseAddress, sizeof(int), 0);
    ReadProcessMemory(handle, (int *)(baseAddress + Chain::offset2), &baseAddress, sizeof(int), 0);
    ReadProcessMemory(handle, (int *)(baseAddress + Chain::offset3), &baseAddress, sizeof(int), 0);
    ReadProcessMemory(handle, (int *)(baseAddress + Chain::offset4), &baseAddress, sizeof(int), 0);

    this->baseAddress = baseAddress;
    float check;

    ReadProcessMemory(handle, (int *)(baseAddress + Camera::zoom), &check, sizeof(float), 0);
    if ((1 <= check) && (check <= 250) && (check == ((int) check)))
    {
        ReadProcessMemory(handle, (int *)(baseAddress + Camera::const12), &check, sizeof(float), 0);
        if (check == 12)
        {
            ReadProcessMemory(handle, (int *)(baseAddress + Camera::fog), &check, sizeof(float), 0);
            if ((0 <= check) && (check <= 100))
            {
                if(QRegExp("^[\\w-]+$").indexIn(getCityName()) != -1) {
                    return true;
                }
            }
        }
    }
    return false;
}

QVariantMap Chain::fromMemoryValues()
{
    QVariantMap cameraValues;
    float rVal;

    ReadProcessMemory(handle, (int *)(baseAddress + Camera::x), &rVal, sizeof(float), 0);
    cameraValues.insert("x", rVal);
    ReadProcessMemory(handle, (int *)(baseAddress + Camera::y), &rVal, sizeof(float), 0);
    cameraValues.insert("y", rVal);
    ReadProcessMemory(handle, (int *)(baseAddress + Camera::z), &rVal, sizeof(float), 0);
    cameraValues.insert("z", rVal);
    ReadProcessMemory(handle, (int *)(baseAddress + Camera::oX), &rVal, sizeof(float), 0);
    cameraValues.insert("oX", rVal);
    ReadProcessMemory(handle, (int *)(baseAddress + Camera::oY), &rVal, sizeof(float), 0);
    cameraValues.insert("oY", rVal);
    ReadProcessMemory(handle, (int *)(baseAddress + Camera::zoom), &rVal, sizeof(float), 0);
    cameraValues.insert("zoom", rVal);
    ReadProcessMemory(handle, (int *)(baseAddress + Camera::exZoom), &rVal, sizeof(float), 0);
    cameraValues.insert("exZoom", rVal);
    ReadProcessMemory(handle, (int *)(baseAddress + Camera::fog), &rVal, sizeof(float), 0);
    cameraValues.insert("fog", rVal);
    cameraValues.insert("cityName", getCityName());
    return cameraValues;
}

QString Chain::getCityName() {
    int citySrc;
    char cityByte = 0;
    int cityPos = 0;
    char cityName[256];
    ReadProcessMemory(handle, (int *)(threadStack + offset0), &citySrc, sizeof(int), 0);
    citySrc += Chain::citOffset1;
    while (1) {
        ReadProcessMemory(handle, (int *)(citySrc + cityPos), &cityByte, sizeof(char), 0);
        cityName[cityPos++] = cityByte;
        if(cityByte == 0x0) {
            break;
        }
        if(cityPos > 255) {
            break;
        }
    }
    return QString(cityName);
}

void Chain::fromBridge(QVariantMap map)
{
    objWriteToMemory = map;
    writeToMemory();
    //    qDebug() << objWriteToMemory;
}

void Chain::writeToMemory()
{
    QVariantMap o = objWriteToMemory;
    if(objWriteToMemory.isEmpty()) {
        return;
    }
    float value;
    value = (float) o.value("x").toString().toFloat();
    WriteProcessMemory(handle, (int *)(baseAddress + Camera::x), &value, sizeof(float), 0);
    value = o.value("y").toString().toFloat();
    WriteProcessMemory(handle, (int *)(baseAddress + Camera::y), &value, sizeof(float), 0);
    value = o.value("z").toString().toFloat();
    WriteProcessMemory(handle, (int *)(baseAddress + Camera::z), &value, sizeof(float), 0);
    value = o.value("oX").toString().toFloat();
    WriteProcessMemory(handle, (int *)(baseAddress + Camera::oX), &value, sizeof(float), 0);
    value = o.value("oY").toString().toFloat();
    WriteProcessMemory(handle, (int *)(baseAddress + Camera::oY), &value, sizeof(float), 0);
    value = o.value("zoom").toString().toFloat();
    WriteProcessMemory(handle, (int *)(baseAddress + Camera::zoom), &value, sizeof(float), 0);
    value = o.value("exZoom").toString().toFloat();
    WriteProcessMemory(handle, (int *)(baseAddress + Camera::exZoom), &value, sizeof(float), 0);
    value = o.value("fog").toString().toFloat();
    WriteProcessMemory(handle, (int *)(baseAddress + Camera::fog), &value, sizeof(float), 0);
}
