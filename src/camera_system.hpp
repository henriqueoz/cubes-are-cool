#ifndef CAMERA_SYSTEM_H_
#define CAMERA_SYSTEM_H_

#include <raylib.h>
#include <raymath.h>

#include "camera.hpp"
#include "isystem.hpp"

class CameraSystem : public ISystem
{
  public:
    CameraSystem() = default;
    ~CameraSystem() = default;
};

#endif // CAMERA_SYSTEM_H_