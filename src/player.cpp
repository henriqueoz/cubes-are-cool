#include <algorithm>
#include <cmath>
#include <cstdint>

#include "player.hpp"

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

    camera.front = Vector3Normalize({direction.x, camera.front.y, direction.z});

    camera.view.target = Vector3Add(camera.view.position, direction);
}

static inline void handle_movement(const float deltaTime, Body3D &body, const PlayerCamera &camera)
{
    const float speed = 5.0f * deltaTime;

    const std::int8_t foward = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
    const std::int8_t sideway = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    const std::int8_t upward = IsKeyDown(KEY_SPACE) - (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_SHIFT));

    if (foward)
    {
        const Vector3 fowardMovement = Vector3Scale(camera.front, speed * foward);

        body.position = Vector3Add(body.position, fowardMovement);
    }

    if (sideway)
    {
        const Vector3 right = Vector3CrossProduct(camera.front, camera.up);
        const Vector3 sideMovement = Vector3Scale(right, speed * sideway);

        body.position = Vector3Add(body.position, sideMovement);
    }

    if (upward)
    {
        const Vector3 upwardMovement = Vector3Scale(camera.up, speed * upward);

        body.position = Vector3Add(body.position, upwardMovement);
    }
}

static inline void player_update_body(const float deltaTime, Body3D &body, const PlayerCamera &camera)
{
    handle_movement(deltaTime, body, camera);

    const float gravitySpeed = body.mass * 9.8f * deltaTime;

    // body.position.y -= gravitySpeed;
}

static Model get_player_model()
{
    Mesh mesh = GenMeshCube(2.0f, 2.0f, 2.0f);

    return LoadModelFromMesh(mesh);
}

Player player_create()
{
    Player player{};

    const Vector3 position = Vector3{0.0f, 0.0f, -20.0f};

    player.body = {.position = position,
                   .front = Vector3{0.0f, 0.0f, 1.0f},
                   .model = get_player_model(),
                   .boundingBox = BoundingBox(position, Vector3{2.0f, 2.0f, 2.0f}),
                   .mass = 1.0f};

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
                .position = Vector3Subtract(position, Vector3{0.0f, 0.0f, 20.0f}),
                .target = Vector3Add(player.body.position, Vector3{0.0f, 0.0f, 1.0f}),
                .up = Vector3{0.0f, 1.0f, 0.0f},
                .fovy = 90.0f,
                .projection = CAMERA_CUSTOM,
            },
    };

    return player;
}

void player_update(const float deltaTime, Player &player)
{
    player_update_camera(player.camera, player.body.position);
    player_update_body(deltaTime, player.body, player.camera);
}

void player_draw(const Player &player)
{
    player.body.boundingBox.render();
}

void player_destroy(Player &player)
{
}