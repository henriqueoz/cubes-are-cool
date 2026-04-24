#include <raylib.h>

#include "game_manager.hpp"
#include "player.hpp"

int main(void)
{
    InitWindow(1280, 720, "Coolest window");
    DisableCursor();

    GameManager gameManager;
    Player player = player_create();

    Mesh cubeMesh = GenMeshCube(10.0f, 10.0f, 10.0f);
    Model cubeModel = LoadModelFromMesh(cubeMesh);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        player_update(player);

        BeginDrawing();
        ClearBackground({20, 20, 20, 255});

        BeginMode3D(player.camera.view);

        DrawModel(cubeModel, Vector3(), 1.0f, BLANK);

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    player_destroy(player);
    UnloadModel(cubeModel);
    EnableCursor();
    CloseWindow();

    return 0;
}
