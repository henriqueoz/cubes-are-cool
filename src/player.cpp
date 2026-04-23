#include <cstdint>

#include "player.hpp"

static Vector3 cameraFront = {0.0f, 0.0f, 1.0f};
static const Vector3 cameraUp = {0.0f, 1.0f, 0.0f};

static void player_update_camera(Player &player)
{
    const Vector2 mouseDelta = GetMouseDelta();
    const float sensitivity = 0.1f;

    player.camera.view.= player.body.position;

    cameraLook.yaw += mouseDelta.x * sensitivity;
    cameraLook.pitch -= mouseDelta.y * sensitivity;

    Vector3 direction{};

    cameraLook.pitch = std::clamp(cameraLook.pitch, -89.0f, 89.0f);

    if (abs(cameraLook.yaw) > 360.0f)
        cameraLook.yaw = 0.0f;

    direction.x = cos(cameraLook.yaw * DEG2RAD) * cos(cameraLook.pitch * DEG2RAD);
    direction.y = sin(cameraLook.pitch * DEG2RAD);
    direction.z = sin(cameraLook.yaw * DEG2RAD) * cos(cameraLook.pitch * DEG2RAD);

    cameraFront = Vector3Normalize({direction.x, cameraFront.y, direction.z});

    camera.target = Vector3Add(camera.position, direction);
}

static inline void player_update_body(Player &player)
{
    const float deltaTime = GetFrameTime();
    const float speed = 5.0f * deltaTime;

    const std::int8_t foward = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
    const std::int8_t sideway = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    const std::int8_t upward = IsKeyDown(KEY_SPACE) - (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_SHIFT));

    if (foward)
    {
        const Vector3 fowardMovement = Vector3Scale(cameraFront, speed * foward);

        body.position = Vector3Add(body.position, fowardMovement);
    }

    if (sideway)
    {
        const Vector3 right = Vector3CrossProduct(cameraFront, cameraUp);
        const Vector3 sideMovement = Vector3Scale(right, speed * sideway);

        body.position = Vector3Add(body.position, sideMovement);
    }

    if (upward)
    {
        const Vector3 upwardMovement = Vector3Scale(cameraUp, speed * upward);

        body.position = Vector3Add(body.position, upwardMovement);
    }
}

Player player_create()
{
    return Player{.camera = {.front = Vector3()}};
}

void player_update(Player &player)
{
    player_update_camera(player.camera);
}