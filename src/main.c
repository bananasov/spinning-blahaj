#include <raylib.h>
#include <raymath.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

#define MAX_CAMERA_PRESETS  2

typedef struct Blahaj
{
    Model model;
    Vector3 position;
    Vector3 rotation_axis;
    float rotation_angle;
    float rotation_speed;
    Vector3 scale;
} Blahaj;

typedef struct CameraPreset
{
    const char* preset_name;
    Vector3 position;
    Vector3 target;
    Vector3 up;
} CameraPreset;

void SelectPreset(Camera* camera, const CameraPreset* preset)
{
    TraceLog(LOG_INFO, TextFormat("BLAHAJ: Loading preset: %s", preset->preset_name));

    camera->position = preset->position;
    camera->target = preset->target;
    camera->up = preset->up;
}

// AI wrote this for me lol, im not doing quaternions
void RotateBlahaj(Blahaj* blahaj)
{
    // Calculate the new rotation angle
    const float newYAngle = blahaj->rotation_axis.y + blahaj->rotation_speed * GetFrameTime();

    // Create quaternions for start and end positions
    const Quaternion start = QuaternionFromEuler(blahaj->rotation_axis.z, blahaj->rotation_axis.y,
                                                 blahaj->rotation_axis.x);
    const Quaternion end = QuaternionFromEuler(0, newYAngle, 0); // Rotate around Y-axis

    // Slerp the quaternions to get the smooth transition
    const Quaternion slerp = QuaternionSlerp(start, end, blahaj->rotation_speed * GetFrameTime());

    // Apply the new rotation to the model
    const Matrix modelTransform = QuaternionToMatrix(slerp);
    blahaj->model.transform = modelTransform;

    // Update the model's rotation
    blahaj->rotation_axis.y = newYAngle;
}

int main(int argc, char** argv)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "SPINNING BLUE SHARK LETS GO");

    struct nk_context *ctx = InitNuklear(10);

    Camera camera = { 0 };
    camera.fovy = 45.0f; // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera mode type

    const CameraPreset camera_presets[MAX_CAMERA_PRESETS] = {
        {"Default", (Vector3){30.0f, 15.0f, 0.0f}, (Vector3){0.0f, 10.0f, 0.0f}, (Vector3){ 0.0f, 1.0f, 0.0f }},
        {"Top down", (Vector3){30.0f, 40.0f, 0.0f}, (Vector3){0.0f, 10.0f, 0.0f}, (Vector3){ 0.0f, 1.0f, 0.0f }}
    };

    SelectPreset(&camera, &camera_presets[0]);

    const Model shark_model = LoadModel("assets/blahaj.obj");
    const Texture2D shark_texture = LoadTexture("assets/blahaj.png");
    shark_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = shark_texture;

    Blahaj shark = {
        .model = shark_model,
        .position = (Vector3){0.0f, 10.0f, 0.0f},
        .rotation_axis = (Vector3){0.0f, 0.0f, 0.0f},
        .rotation_speed = 1.0f,
        .scale = (Vector3){10.0f, 10.0f, 10.0f},
    };

    int currently_selected_preset = 0;
    bool cursor_disabled = true;
    bool ui_enabled = true;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateNuklear(ctx);

        if (!cursor_disabled) UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed(KEY_F)) {
            cursor_disabled = !cursor_disabled;
            cursor_disabled ? EnableCursor() : DisableCursor();
        }

        if (IsKeyPressed(KEY_P)) ui_enabled = !ui_enabled;

        if (IsKeyPressed(KEY_PAGE_UP)) {
            currently_selected_preset = (currently_selected_preset + 1) % MAX_CAMERA_PRESETS;
            SelectPreset(&camera, &camera_presets[currently_selected_preset]);
        }

        if (IsKeyPressed(KEY_PAGE_DOWN)) {
            currently_selected_preset = (currently_selected_preset - 1) % MAX_CAMERA_PRESETS;
            SelectPreset(&camera, &camera_presets[currently_selected_preset]);
        }

        if (IsKeyDown(KEY_UP)) shark.rotation_speed += 1.0f;
        if (IsKeyDown(KEY_DOWN)) shark.rotation_speed -= 1.0f;

        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (ui_enabled) {
                if (nk_begin(ctx, "Blahaj", nk_rect(100, 100, 220, 220),
                    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
                    nk_layout_row_static(ctx, 50, 150, 1);

                    if (nk_button_label(ctx, "Button")) {
                        // Button was clicked!
                    }
                }
                nk_end(ctx);
            }

            BeginMode3D(camera);

                DrawModelEx(shark.model, shark.position, shark.rotation_axis, shark.rotation_angle, shark.scale, WHITE);
                RotateBlahaj(&shark);

                DrawGrid(20, 10.0f);

            EndMode3D();

            if (ui_enabled) {
                DrawNuklear(ctx);

                // TODO: Make positioning better than hardcoding this bullshit lol
                DrawText("Press F to enable/disable focus", 5, 0, 20, LIGHTGRAY);
                DrawText("Press P to toggle UI", 5, 20, 20, LIGHTGRAY);
                DrawText(TextFormat("Camera Position: %.2f, %.2f, %.2f", camera.position.x, camera.position.y,
                                    camera.position.z), 5, 40, 20, LIGHTGRAY);
                DrawText(TextFormat("Camera Target: %.2f, %.2f, %.2f", camera.target.x, camera.target.y,
                                    camera.target.z), 5, 60, 20, LIGHTGRAY);
                DrawText(TextFormat("Camera Up: %.2f, %.2f, %.2f", camera.up.x, camera.up.y,
                                    camera.up.z), 5, 80, 20, LIGHTGRAY);
                DrawText(TextFormat("Camera Preset: %s", camera_presets[currently_selected_preset].preset_name), 5,
                                    100, 20, LIGHTGRAY);
                DrawText(TextFormat("Rotation speed: %.2f", shark.rotation_speed), 5, 120, 20, LIGHTGRAY);
            }

        EndDrawing();
    }

    UnloadTexture(shark_texture);
    UnloadModel(shark_model);

    UnloadNuklear(ctx);

    CloseWindow();

    return 0;
}
