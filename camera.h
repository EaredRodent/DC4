#ifndef CAMERA_H
#define CAMERA_H


class Camera
{
public:
    Camera();
    static const int const12 = 0x58;
    static const int zoom = 0x50; // 0 to 250         (default 25)
    static const int y = 0x5c; // -200 to 200      (default 25)
    static const int x = 0x70; // -40 to 40        (default 0)
    static const int z = 0x74; // -40 to 40        (default 0.5)

    static const int oY = 0x64; // -1.57 to 0.5     (default -0.78)
    static const int oX = 0x68; // -3.14 to 3.14    (default -0.78)

    static const int fog = 0x60; // 0 to 25          (default 25)
    static const int exZoom = 0x6C; // 0.59 to 2.5      (default 0.59)
};

#endif // CAMERA_H
