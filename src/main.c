#include <raylib.h>
#include <raymath.h>

typedef struct Blahaj {
    Model model;
    Vector3 position;
    Vector3 rotation_axis;
    float rotation_angle;
    float rotation_speed;
    Vector3 scale;
} Blahaj;

// AI wrote this for me lol, im not doing quaternions
void RotateBlahaj(Blahaj* blahaj) {
    // Calculate the new rotation angle
    const float newYAngle = blahaj->rotation_axis.y + blahaj->rotation_speed * GetFrameTime();

    // Create quaternions for start and end positions
    const Quaternion start = QuaternionFromEuler(blahaj->rotation_axis.z, blahaj->rotation_axis.y, blahaj->rotation_axis.x);
    const Quaternion end = QuaternionFromEuler(0, newYAngle, 0); // Rotate around Y-axis

    // Slerp the quaternions to get the smooth transition
    const Quaternion slerp = QuaternionSlerp(start, end, blahaj->rotation_speed * GetFrameTime());

    // Apply the new rotation to the model
    const Matrix modelTransform = QuaternionToMatrix(slerp);
    blahaj->model.transform = modelTransform;

    // Update the model's rotation
    blahaj->rotation_axis.y = newYAngle;
}

int main(int argc, char** argv) {
    InitWindow(800, 600, "SPINNING BLUE SHARK LETS GO");

    Camera camera = { 0 };
    camera.position = (Vector3){ 50.0f, 50.0f, 50.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 10.0f, 0.0f };     // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    const Model shark_model = LoadModel("assets/blahaj.obj");
    const Texture2D shark_texture = LoadTexture("assets/blahaj.png");
    shark_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = shark_texture; // set the blahaj texture, no idea if this is gonna work but i hope it will

    Blahaj shark = {
        .model = shark_model,
        .position = (Vector3){ 0.0f, 0.0f, 0.0f },
        .rotation_axis = (Vector3){ 0.0f, 0.0f, 0.0f },
        .rotation_speed = 1.0f,
        .scale = (Vector3){ 10.0f, 10.0f, 10.0f },
    };

    bool cursor_disabled = true;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (!cursor_disabled) UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed(KEY_F)) {
            cursor_disabled = !cursor_disabled;
            cursor_disabled? EnableCursor() : DisableCursor();
        }

        if (IsKeyDown(KEY_UP)) shark.rotation_speed += 1.0f;
        if (IsKeyDown(KEY_DOWN)) shark.rotation_speed -= 1.0f;
    
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawModelEx(shark.model, shark.position, shark.rotation_axis, shark.rotation_angle, shark.scale, WHITE);
                RotateBlahaj(&shark);
                
                DrawGrid(20, 10.0f);

            EndMode3D();

            DrawText("Press F to enable/disable focus", 0, 0, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
