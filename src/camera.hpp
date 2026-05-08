#ifndef CAMERA_H_
#define CAMERA_H_

#include <raylib.h>

namespace engine
{
class Camera
{
  public:
    Camera() = default;
    ~Camera() = default;

  private:
    Vector3 position_;

    Vector3 front_;
    Vector3 up_;

    float yaw_;
    float pitch_;

    Camera3D view_;
};
}; // namespace engine

#endif