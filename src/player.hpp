#ifndef PLAYER_H_
#define PLAYER_H_

#include "raylib.h"

struct Body3D
{
    Vector3 position;
};

struct Camera
{
    Camera3D view;
    struct
    {
        float yaw;
        float pitch;
    } look;
    Vector3 front;
    Vector3 up;
};

struct Player
{
    Body3D body;
    Camera camera;
};

Player player_create();
void player_update(Player &player);

#endif