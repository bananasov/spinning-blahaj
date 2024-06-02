#include "raylib.h"

#include "imgui.h"

#define NO_FONT_AWESOME
#include "rlImgui/rlImGui.h"

#include "Blahaj/Blahaj.h"
#include "CameraPresets.h"

int main(int argc, char** argv) {
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 800;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "SPINNING BLUE SHARK LETS GO");

    Camera camera = {};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    SelectPreset(&camera, &presets[0]); // Default preset

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

        if (IsKeyPressed(KEY_PAGE_UP)) {
            currently_selected_preset = (currently_selected_preset + 1) % MAX_CAMERA_PRESETS;
            SelectPreset(&camera, &presets[currently_selected_preset]);
        }

        if (IsKeyPressed(KEY_PAGE_DOWN)) {
            currently_selected_preset = (currently_selected_preset - 1) % MAX_CAMERA_PRESETS;
            SelectPreset(&camera, &presets[currently_selected_preset]);
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                blahaj->Render();
                blahaj->Rotate();

                DrawGrid(20, 10.0f);
            EndMode3D();

            if (ui_open) {
                rlImGuiBegin();
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {400.f,200.f });

                    ImGui::Begin("Blahaj", &ui_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
                        ImGui::SliderFloat("Rotation Speed", &blahaj->RotationSpeed, 1.0f, 180.0f);
                    ImGui::End();

                    ImGui::Begin("Camera Settings", &ui_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
                        ImGui::SliderFloat("FOV", &camera.fovy, 1.0f, 180.0f);

                        if (ImGui::BeginCombo("##presets", presets[currently_selected_preset].preset_name)) {
                            for (int n = 0; n < IM_ARRAYSIZE(presets); n++) {
                                auto preset = presets[n];
                                const bool is_selected = (currently_selected_preset == n);

                                if (ImGui::Selectable(preset.preset_name, is_selected)) {
                                    currently_selected_preset = n;
                                    SelectPreset(&camera, &presets[currently_selected_preset]);
                                }
                                if (is_selected) ImGui::SetItemDefaultFocus();
                            }

                            ImGui::EndCombo();
                        }
                    ImGui::End();

                    ImGui::PopStyleVar();
                rlImGuiEnd();
            }
        EndDrawing();
    }

    delete blahaj;

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}