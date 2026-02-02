#include <algorithm>
#include <cstdint>

#include "raylib.h"
#include "raymath.h"

struct Body3D {
  Vector3 position;
};

struct CameraLook {
  float yaw;
  float pitch;
};

static CameraLook cameraLook{};
static Vector3 cameraFront = {0.0f, 0.0f, 1.0f};
static const Vector3 cameraUp = {0.0f, 1.0f, 0.0f};

static void UpdateBody(Body3D &body) {
  const float deltaTime = GetFrameTime();
  const float speed = 5.0f * deltaTime;

  const std::int8_t foward = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
  const std::int8_t sideway = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);

  if (foward) {
    const Vector3 fowardMovement = Vector3Scale(cameraFront, speed * foward);

    body.position = Vector3Add(body.position, fowardMovement);
  }

  if (sideway) {
    const Vector3 right = Vector3CrossProduct(cameraFront, cameraUp);
    const Vector3 sideMovement = Vector3Scale(right, speed * sideway);

    body.position = Vector3Add(body.position, sideMovement);
  }
}

static void UpdateCamera(Camera3D &camera, const Vector3 position) {
  const Vector2 mouseDelta = GetMouseDelta();
  const float sensitivity = 0.1f;

  camera.position = position;

  cameraLook.yaw += mouseDelta.x * sensitivity;
  cameraLook.pitch -= mouseDelta.y * sensitivity;

  Vector3 direction{};

  cameraLook.pitch = std::clamp(cameraLook.pitch, -89.0f, 89.0f);

  if (abs(cameraLook.yaw) >= 360.0f) {
    cameraLook.yaw = 0.0f;
  }

  TraceLog(LOG_INFO, "yaw: %f pitch: %f", cameraLook.yaw, cameraLook.pitch);

  direction.x = cos(cameraLook.yaw * DEG2RAD) * cos(cameraLook.pitch * DEG2RAD);
  direction.y = sin(cameraLook.pitch * DEG2RAD);
  direction.z = sin(cameraLook.yaw * DEG2RAD) * cos(cameraLook.pitch * DEG2RAD);

  cameraFront = Vector3Normalize(direction);

  camera.target = Vector3Add(camera.position, cameraFront);
}

int main(void) {
  Camera3D camera = {};
  camera.position = {0.0f, 0.0f, -20.0f};
  camera.target = {0.0f, 0.0f, 0.0f};
  camera.up = cameraUp;
  camera.fovy = 90.0f;
  camera.projection = CAMERA_CUSTOM;

  Body3D body = {};
  body.position = camera.position;

  InitWindow(800, 600, "Coolest window");

  while (!WindowShouldClose()) {
    UpdateBody(body);
    UpdateCamera(camera, body.position);

    if (IsCursorOnScreen()) {
      if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        DisableCursor();
      } else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        EnableCursor();
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);

    DrawCube({0.0f}, 10.0f, 10.f, 10.0f, PINK);

    EndMode3D();
    EndDrawing();
  }

  return 0;
}
