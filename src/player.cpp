#include <algorithm>
#include <cmath>
#include <cstdint>

#include "raylib.h"

#include "player.hpp"

static Vector3 cameraFront = {0.0f, 0.0f, 1.0f};
static const Vector3 cameraUp = {0.0f, 1.0f, 0.0f};

static void player_update_camera(PlayerCamera &camera, const Vector3 position)
{
    const Vector2 mouseDelta = GetMouseDelta();
    const float sensitivity = 0.1f;

    camera.view.position = position;

    camera.look.yaw += mouseDelta.x * sensitivity;
    camera.look.pitch -= mouseDelta.y * sensitivity;

    Vector3 direction{};

    camera.look.pitch = std::clamp(camera.look.pitch, -89.0f, 89.0f);

    if (abs(camera.look.yaw) > 360.0f)
        camera.look.yaw = 0.0f;

    direction.x = cos(camera.look.yaw * DEG2RAD) * cos(camera.look.pitch * DEG2RAD);
    direction.y = sin(camera.look.pitch * DEG2RAD);
    direction.z = sin(camera.look.yaw * DEG2RAD) * cos(camera.look.pitch * DEG2RAD);

    cameraFront = Vector3Normalize({direction.x, cameraFront.y, direction.z});

    camera.view.target = Vector3Add(camera.view.position, direction);
}

static inline void player_update_body(Body3D &body)
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

static Model get_player_model()
{
    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);

    return LoadModelFromMesh(mesh);
}

Player player_create()
{
    Player player{};

    player.body = {
        .position = {0.0f, 0.0f, -20.0f},
        .model = get_player_model(),
    };

    player.camera = {
        .front = Vector3{0.0f, 0.0f, 1.0f},
        .up = Vector3{0.0f, 1.0f, 0.0f},
        .look =
            {
                .yaw = 0.0f,
                .pitch = 0.0f,
            },
        .view =
            {
                .position = player.body.position,
                .target = Vector3Add(player.body.position, Vector3{0.0f, 0.0f, 1.0f}),
                .up = Vector3{0.0f, 1.0f, 0.0f},
                .fovy = 90.0f,
                .projection = CAMERA_CUSTOM,
            },
    };

    return player;
}

void player_update(Player &player)
{
    player_update_camera(player.camera, player.body.position);
    player_update_body(player.body);
}

void player_draw(const Player &player)
{
}

void player_destroy(Player &player)
{
    UnloadModel(player.body.model);
}