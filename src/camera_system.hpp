#ifndef CAMERA_SYSTEM_H_
#define CAMERA_SYSTEM_H_

#include <raylib.h>
#include <raymath.h>

#include "isystem.hpp"

struct CameraState
{
    Vector3 front;
    Vector3 up;
    struct
    {
        float yaw;
        float pitch;
    } look;
    Camera3D view;
};

class CameraSystem : public ISystem
{
  public:
    CameraSystem() = default;
    ~CameraSystem() = default;
};

#endif // CAMERA_SYSTEM_H_