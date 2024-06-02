#include "raylib.h"
#include "raymath.h"

#include "imgui.h"

#define NO_FONT_AWESOME
#include "rlImgui/rlImGui.h"

#include "blahaj/Blahaj.h"

int main(int argc, char** argv) {
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "SPINNING BLUE SHARK LETS GO");

    Camera camera = {};
    camera.position = (Vector3){ 30.0f, 15.0f, 0.0f };
    camera.target = (Vector3){ 0.0f, 10.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    const Model sharkModel = LoadModel("assets/blahaj.obj");
    const Texture2D sharkTexture = LoadTexture("assets/blahaj.png");

    const auto blahaj = new Blahaj(sharkModel, sharkTexture, (Vector3){ 0.0f, 10.0f, 0.0f });

    SetTargetFPS(60);
    rlImGuiSetup(true);

    bool ui_open = true;
    bool cursor_disabled = true;

    while (!WindowShouldClose()) {
        if (!cursor_disabled) UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed(KEY_F)) {
            cursor_disabled = !cursor_disabled;
            cursor_disabled ? EnableCursor() : DisableCursor();
        }

        if (IsKeyPressed(KEY_P)) ui_open = !ui_open;

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                blahaj->Render();
                blahaj->Rotate();

                DrawGrid(20, 10.0f);
            EndMode3D();

            if (ui_open) {
                rlImGuiBegin();

                    if (ImGui::Begin("Blahaj", &ui_open, ImGuiWindowFlags_NoScrollbar))
                    {
                        ImGui::SliderFloat("Rotation Speed", &blahaj->RotationSpeed, 1.0f, 180.0f);
                    }

                rlImGuiEnd();
            }
        EndDrawing();
    }

    delete blahaj;

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}