#ifndef PLAYER_H_
#define PLAYER_H_

#include "raylib.h"

struct Body3D
{
    Vector3 position;
    Model model;
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
void player_update(Player &player);
void player_draw(const Player &player);
void player_destroy(Player &player);

#endif // PLAYER_H_