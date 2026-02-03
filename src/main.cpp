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

static bool isPaused = false;

static void UpdateBody(Body3D &body) {
  const float deltaTime = GetFrameTime();
  const float speed = 5.0f * deltaTime;

  const std::int8_t foward = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
  const std::int8_t sideway = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
  const std::int8_t upward =
      IsKeyDown(KEY_SPACE) -
      (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_SHIFT));

  if (foward) {
    const Vector3 fowardMovement = Vector3Scale(cameraFront, speed * foward);

    body.position = Vector3Add(body.position, fowardMovement);
  }

  if (sideway) {
    const Vector3 right = Vector3CrossProduct(cameraFront, cameraUp);
    const Vector3 sideMovement = Vector3Scale(right, speed * sideway);

    body.position = Vector3Add(body.position, sideMovement);
  }

  if (upward) {
    const Vector3 upwardMovement = Vector3Scale(cameraUp, speed * upward);

    body.position = Vector3Add(body.position, upwardMovement);
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

  if (abs(cameraLook.yaw) > 360.0f) cameraLook.yaw = 0.0f;

  direction.x = cos(cameraLook.yaw * DEG2RAD) * cos(cameraLook.pitch * DEG2RAD);
  direction.y = sin(cameraLook.pitch * DEG2RAD);
  direction.z = sin(cameraLook.yaw * DEG2RAD) * cos(cameraLook.pitch * DEG2RAD);

  cameraFront = Vector3Normalize({direction.x, cameraFront.y, direction.z});

  camera.target = Vector3Add(camera.position, direction);
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

  InitWindow(1280, 720, "Coolest window");

  Texture2D faceTex = LoadTexture("resources/awesomeface.png");
  GenTextureMipmaps(&faceTex);

  while (!WindowShouldClose()) {
    if (IsCursorOnScreen()) {
      if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isPaused = false;
        DisableCursor();
      } else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        isPaused = true;
        EnableCursor();
      }
    }

    if (!isPaused) {
      UpdateBody(body);
      UpdateCamera(camera, body.position);
    }

    BeginDrawing();
    ClearBackground({20, 20, 20, 255});

    if (isPaused) {
      DrawRectangle(0, 0, 1280, 720, ColorAlpha(GRAY, 0.5f));
      DrawText("PAUSED", 0, 0, 24, WHITE);
    }

    BeginMode3D(camera);

    RenderTexture2D();
    DrawCube({0.0f, 5.0f, 0.0f}, 10.0f, 10.0f, 10.0f, PINK);

    DrawPlane({0.0f, -0.001f, 0.0f}, {200.0f, 200.0f}, ColorAlpha(GRAY, 0.5f));

    DrawLine3D({0}, {0.0f, 100.0f, 0.0f}, GREEN);
    DrawLine3D({0}, {0.0f, -100.0f, 0.0f}, DARKGREEN);

    DrawLine3D({0}, {100.0f, 0.0f, 0.0f}, RED);
    DrawLine3D({0}, {-100.0f, 0.0f, 0.0f}, PINK);

    DrawLine3D({0}, {0.0f, 0.0f, 100.0f}, BLUE);
    DrawLine3D({0}, {0.0f, 0.0f, -100.0f}, DARKBLUE);

    EndMode3D();
    EndDrawing();
  }

  return 0;
}
