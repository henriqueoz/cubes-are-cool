#include <algorithm>
#include <cmath>
#include <cstdint>

#include "external/glad.h"
#include "raylib.h"
#include "raymath.h"

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

    Texture2D containerTex = LoadTexture("./resources/textures/container.jpg");
    GenTextureMipmaps(&containerTex);

    const Mesh cubeMesh = GenMeshCube(10.0f, 10.0f, 10.0f);
    const Model cubeModel = LoadModelFromMesh(cubeMesh);

    // SetMaterialTexture(&cubeModel.materials[0], MATERIAL_MAP_DIFFUSE, containerTex);

    const Shader lightShader = LoadShader("./resources/shaders/light.vert", "./resources/shaders/light.frag");
    const Shader sourceShader = LoadShader("./resources/shaders/source.vert", "./resources/shaders/source.frag");

    const int objectColorLoc = GetShaderLocation(lightShader, "objectColor");
    const int loightColorLoc = GetShaderLocation(lightShader, "lightColor");

    cubeModel.materials[0].shader = lightShader;

    const Vector3 lightPos = Vector3Scale({ 1.2f, 1.0f, 2.0f }, 5);

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

            SetShaderValueV(lightShader, objectColorLoc, objectColor, SHADER_UNIFORM_VEC3, 1);
            SetShaderValue(lightShader, loightColorLoc, lightColor, SHADER_UNIFORM_VEC3);

            float aspect = (float)GetScreenWidth() / (float)GetScreenHeight();

            Matrix projection = MatrixPerspective(camera.fovy * DEG2RAD, aspect, 0.1f, 100.0f);
            Matrix view = GetCameraMatrix(camera);

            Matrix model = MatrixIdentity();
            Matrix lightModel = MatrixTranslate(lightPos.x, lightPos.y, lightPos.z);

            SetShaderValueMatrix(lightShader, GetShaderLocation(lightShader, "projection"), projection);
            SetShaderValueMatrix(lightShader, GetShaderLocation(lightShader, "view"), view);
            SetShaderValueMatrix(lightShader, GetShaderLocation(lightShader, "model"), model);

            SetShaderValueMatrix(sourceShader, GetShaderLocation(sourceShader, "projection"), projection);
            SetShaderValueMatrix(sourceShader, GetShaderLocation(sourceShader, "view"), view);
            SetShaderValueMatrix(sourceShader, GetShaderLocation(sourceShader, "model"), lightModel);

            BeginMode3D(camera);

                BeginShaderMode(lightShader);
                    DrawModel(cubeModel, Vector3Zero(), 1.0f, BLANK);
                EndShaderMode();

                BeginShaderMode(sourceShader);
                    DrawCube(lightPos, 2.0f, 2.0f, 2.0f, BLANK);
                EndShaderMode();

            EndMode3D();
        EndDrawing();
    }

    UnloadTexture(containerTex);
    UnloadModel(cubeModel);

    return 0;
}
