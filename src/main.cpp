#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <math.h>

#include "external/glfw/deps/glad/gl.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "shader.hpp"

namespace fs = std::filesystem;

struct Body3D
{
    Vector3 position;
};

struct CameraLook
{
    float yaw;
    float pitch;
};

static bool isPaused = false;

static fs::path GetExecutablePath()
{
    const char *path = GetApplicationDirectory();

    return fs::path(path);
}

int main(void)
{
    Camera3D camera = {};
    camera.position = {0.0f, 0.0f, -20.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = cameraUp;
    camera.fovy = 90.0f;
    camera.projection = CAMERA_CUSTOM;

    Body3D body = {};
    body.position = camera.position;

    InitWindow(1280, 720, "Coolest window");

    fs::path resources = GetExecutablePath() / "resources";
    fs::path shaderPath = resources / "shaders";

    const Shader lightShader = LoadShader((shaderPath / "light.vert").c_str(), (shaderPath / "light.frag").c_str());
    const Shader sourceShader = LoadShader((shaderPath / "source.vert").c_str(), (shaderPath / "source.frag").c_str());

    const int lightColorLoc = GetShaderLocation(lightShader, "lightColor");
    const int lightPosLoc = GetShaderLocation(lightShader, "lightPos");
    const int viewPosLoc = GetShaderLocation(lightShader, "viewPos");

    const auto cubePhong = GetPhongShaderLocs(lightShader);

    const float orbitRadius = 15.0f;

    Vector3 lightPos = {orbitRadius, 6.0f, 0.0f};

    Mesh cubeMesh = GenMeshCube(10.0f, 10.0f, 10.0f);
    Mesh sourceMesh = GenMeshCube(2.0f, 2.0f, 2.0f);

    Model cubeModel = LoadModelFromMesh(cubeMesh);
    Model sourceModel = LoadModelFromMesh(sourceMesh);

    cubeModel.materials[0].shader = lightShader;
    cubeModel.materials[0].shader.locs[SHADER_LOC_MATRIX_PROJECTION] = GetShaderLocation(lightShader, "projection");
    cubeModel.materials[0].shader.locs[SHADER_LOC_MATRIX_VIEW] = GetShaderLocation(lightShader, "view");
    cubeModel.materials[0].shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(lightShader, "model");

    sourceModel.materials[0].shader = sourceShader;
    sourceModel.materials[0].shader.locs[SHADER_LOC_MATRIX_PROJECTION] = GetShaderLocation(sourceShader, "projection");
    sourceModel.materials[0].shader.locs[SHADER_LOC_MATRIX_VIEW] = GetShaderLocation(sourceShader, "view");
    sourceModel.materials[0].shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(sourceShader, "model");

    SetTargetFPS(60);

    ShaderValues cubeShaderValues;
    cubeShaderValues.material = {.diffuse = {1.0f, 0.5f, 0.31f}, .specular = {0.5f, 0.5f, 0.5f}, .shineness = 8.0f};
    cubeShaderValues.light = {.position = lightPos,
                              .ambient = {0.1f, 0.7f, 0.2f},
                              .diffuse = {0.5f, 0.5f, 0.5f},
                              .specular = {1.0f, 1.0f, 1.0f}};

    SetPhongShaderValues(cubePhong, cubeShaderValues);

    while (!WindowShouldClose())
    {
        if (IsCursorOnScreen())
        {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                isPaused = false;
                DisableCursor();
            }
            else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
            {
                isPaused = true;
                EnableCursor();
            }
        }

        if (!isPaused)
        {
            UpdateBody(body);
            UpdateCamera(camera, body.position);
        }

        BeginDrawing();
        ClearBackground({20, 20, 20, 255});

        if (isPaused)
        {
            DrawRectangle(0, 0, 1280, 720, ColorAlpha(GRAY, 0.5f));
            DrawText("PAUSED", 0, 0, 24, WHITE);
        }

        float objectColor[3] = {1.0f, 0.5f, 0.31f};
        float lightColor[3] = {1.0f, 1.0f, 1.0f};

        SetShaderValue(lightShader, lightColorLoc, lightColor, SHADER_UNIFORM_VEC3);
        SetShaderValue(lightShader, lightPosLoc, Vector3ToFloat(lightPos), SHADER_UNIFORM_VEC3);
        SetShaderValue(lightShader, viewPosLoc, Vector3ToFloat(camera.position), SHADER_UNIFORM_VEC3);

        const float speed = 1.5f;
        const float angle = GetTime() * speed;

        lightPos.x = orbitRadius * cosf(angle);
        lightPos.z = orbitRadius * sinf(angle);

        cubeShaderValues.light.position = lightPos;
        SetPhongShaderValues(cubePhong, cubeShaderValues);

        BeginMode3D(camera);

        DrawModel(cubeModel, Vector3Zero(), 1.0f, BLANK);
        DrawModel(sourceModel, lightPos, 1.0f, BLANK);

        EndMode3D();
        EndDrawing();
    }

    UnloadModel(cubeModel);
    UnloadModel(sourceModel);

    UnloadShader(lightShader);
    UnloadShader(sourceShader);

    return 0;
}
