#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#include "external/glfw/deps/glad/gl.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

struct Body3D
{
    Vector3 position;
};

struct CameraLook
{
    float yaw;
    float pitch;
};

static CameraLook cameraLook{};
static Vector3 cameraFront = { 0.0f, 0.0f, 1.0f };
static const Vector3 cameraUp = { 0.0f, 1.0f, 0.0f };

static bool isPaused = false;

static Matrix MatrixFill(float v) {
    return {
        v, v, v, v,
        v, v, v, v,
        v, v, v, v,
        v, v, v, v
    };
}

static void
UpdateBody(Body3D & body)
{
    const float deltaTime = GetFrameTime();
    const float speed = 5.0f * deltaTime;

    const std::int8_t foward = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
    const std::int8_t sideway = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    const std::int8_t upward = IsKeyDown(KEY_SPACE) - (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_SHIFT));

    if (foward) {
        const Vector3 fowardMovement = Vector3Scale(cameraFront, speed * foward);

        body.position = Vector3Add(body.position, fowardMovement);
    }

    if (sideway) {
        const Vector3 right = Vector3CrossProduct(cameraFront, cameraUp);
        const Vector3 sideMovement = Vector3Scale(right, speed * sideway);

        body.position = Vector3Add(body.position, sideMovement);
    }

    if (upward) {
        const Vector3 upwardMovement = Vector3Scale(cameraUp, speed * upward);

        body.position = Vector3Add(body.position, upwardMovement);
    }
}

static void
UpdateCamera(Camera3D & camera, const Vector3 position)
{
    const Vector2 mouseDelta = GetMouseDelta();
    const float sensitivity = 0.1f;

    camera.position = position;

    cameraLook.yaw += mouseDelta.x * sensitivity;
    cameraLook.pitch -= mouseDelta.y * sensitivity;

    Vector3 direction{};

    cameraLook.pitch = std::clamp(cameraLook.pitch, -89.0f, 89.0f);

    if (abs(cameraLook.yaw) > 360.0f)
        cameraLook.yaw = 0.0f;

    direction.x = cos(cameraLook.yaw * DEG2RAD) * cos(cameraLook.pitch * DEG2RAD);
    direction.y = sin(cameraLook.pitch * DEG2RAD);
    direction.z = sin(cameraLook.yaw * DEG2RAD) * cos(cameraLook.pitch * DEG2RAD);

    cameraFront = Vector3Normalize({ direction.x, cameraFront.y, direction.z });

    camera.target = Vector3Add(camera.position, direction);
}

int
main(void)
{
    Camera3D camera = {};
    camera.position = { 0.0f, 0.0f, -20.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = cameraUp;
    camera.fovy = 90.0f;
    camera.projection = CAMERA_CUSTOM;

    Body3D body = {};
    body.position = camera.position;

    InitWindow(1280, 720, "Coolest window");

    const Shader lightShader = LoadShader("./resources/shaders/light.vert", "./resources/shaders/light.frag");
    const Shader sourceShader = LoadShader("./resources/shaders/source.vert", "./resources/shaders/source.frag");

    const int objectColorLoc = GetShaderLocation(lightShader, "objectColor");
    const int lightColorLoc = GetShaderLocation(lightShader, "lightColor");
    const int lightPosLoc = GetShaderLocation(lightShader, "lightPos");
    const int viewPosLoc = GetShaderLocation(lightShader, "viewPos");

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

    while (!WindowShouldClose()) {
        if (IsCursorOnScreen()) {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                isPaused = false;
                DisableCursor();
            } else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                isPaused = true;
                EnableCursor();
            }
        }

        if (!isPaused) {
            UpdateBody(body);
            UpdateCamera(camera, body.position);
        }

        BeginDrawing();
            ClearBackground({ 20, 20, 20, 255 });

            if (isPaused) {
                DrawRectangle(0, 0, 1280, 720, ColorAlpha(GRAY, 0.5f));
                DrawText("PAUSED", 0, 0, 24, WHITE);
            }

            float objectColor[3] = { 1.0f, 0.5f, 0.31f };
            float lightColor[3] = { 1.0f, 1.0f, 1.0f };

            SetShaderValue(lightShader, objectColorLoc, objectColor, SHADER_UNIFORM_VEC3);
            SetShaderValue(lightShader, lightColorLoc, lightColor, SHADER_UNIFORM_VEC3);
            SetShaderValue(lightShader, lightPosLoc, Vector3ToFloat(lightPos), SHADER_UNIFORM_VEC3);
            SetShaderValue(lightShader, viewPosLoc, Vector3ToFloat(camera.position), SHADER_UNIFORM_VEC3);

            const float speed = 1.5f;
            const float angle = GetTime() * speed;

            lightPos.x = orbitRadius * cosf(angle);
            lightPos.z = orbitRadius * sinf(angle);

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