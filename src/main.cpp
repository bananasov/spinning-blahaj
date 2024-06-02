#include "raylib.h"
#include "raymath.h"

#include "imgui.h"

#define NO_FONT_AWESOME
#include "rlImgui/rlImGui.h"

int main(int argc, char** argv) {
    int screenWidth = 800;
    int screenHeight = 600;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "SPINNING BLUE SHARK LETS GO");
    SetTargetFPS(144);
    rlImGuiSetup(true);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(DARKGRAY);

            rlImGuiBegin();

                bool open = true;
                ImGui::ShowDemoWindow(&open);

            rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}