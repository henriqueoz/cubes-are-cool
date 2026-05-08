#include <raylib.h>

#include "camera_system.hpp"
#include "player.hpp"
#include "system_manager.hpp"

int main(void)
{
    InitWindow(1280, 720, "Coolest window");
    DisableCursor();

    SystemManager systemManger;
    systemManger.add(new CameraSystem());

    Player player = player_create();

    Mesh cubeMesh = GenMeshCube(10.0f, 10.0f, 10.0f);
    Model cubeModel = LoadModelFromMesh(cubeMesh);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        player_update(deltaTime, player);

        BeginDrawing();
        ClearBackground({20, 20, 20, 255});

        BeginMode3D(player.camera.view);

        DrawPlane(Vector3Zero(), Vector2{100.0f, 100.0f}, DARKGRAY);

        DrawModel(cubeModel, {0.0f, 5.0f, 0.0f}, 1.0f, BLUE);

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    player_destroy(player);

    UnloadModel(cubeModel);
    CloseWindow();

    return 0;
}
