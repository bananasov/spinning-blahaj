#pragma once

#include <raylib.h>

#define MAX_CAMERA_PRESETS 2

typedef struct CameraPreset
{
    const char* preset_name;
    Vector3 position;
    Vector3 target;
    Vector3 up;
} CameraPreset;

static CameraPreset presets[MAX_CAMERA_PRESETS] = {
    {"Default", (Vector3){30.0f, 15.0f, 0.0f}, (Vector3){0.0f, 10.0f, 0.0f}, (Vector3){ 0.0f, 1.0f, 0.0f }},
    {"Top down", (Vector3){30.0f, 40.0f, 0.0f}, (Vector3){0.0f, 10.0f, 0.0f}, (Vector3){ 0.0f, 1.0f, 0.0f }}
};

static int currently_selected_preset = 0;

inline void SelectPreset(Camera* camera, const CameraPreset* preset)
{
    TraceLog(LOG_INFO, TextFormat("BLAHAJ: Loading preset: %s", preset->preset_name));

    camera->position = preset->position;
    camera->target = preset->target;
    camera->up = preset->up;
}