#ifndef PLAYER_H_
#define PLAYER_H_

#include <cstdint>

#include <raylib.h>
#include <raymath.h>

#include "bounding_box.hpp"

enum class PlayerCameraMode : std::uint8_t
{
    FIRST_PERSON,
    THIRD_PERSON
};

struct Body3D
{
    Vector3 position;
    Vector3 front;
    Model model;
    engine::BoundingBox boundingBox;
    float mass;
};

struct PlayerCameraFirstPerson
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

struct PlayerCameraThirdPerson
{
    float distanceToPlayer;
    Vector3 up;
    struct
    {
        float yaw;
        float pitch;
    } look;
    Camera3D view;
};

struct Player
{
    Body3D body;
    PlayerCameraFirstPerson firstPersonCamera;
    PlayerCameraThirdPerson thirdPersonCamera;
};

Player player_create();
void player_update(const float deltaTime, Player &player);
void player_draw(const Player &player);
void player_destroy(Player &player);

#endif // PLAYER_H_