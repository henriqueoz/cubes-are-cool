#ifndef PLAYER_H_
#define PLAYER_H_

#include <raylib.h>
#include <raymath.h>

#include "bounding_box.hpp"

struct Body3D
{
    Vector3 position;
    Vector3 front;
    Model model;
    BoundingBox boundingBox;
    float mass;
};

struct PlayerCamera
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

struct Player
{
    Body3D body;
    PlayerCamera camera;
};

Player player_create();
void player_update(const float deltaTime, Player &player);
void player_draw(const Player &player);
void player_destroy(Player &player);

#endif // PLAYER_H_