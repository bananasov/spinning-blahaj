#include "Blahaj.h"

#include <raymath.h>

Blahaj::Blahaj(Model model, Texture2D texture) : m_Model(model), m_Texture(texture)
{
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

Blahaj::Blahaj(Model model, Texture2D texture, Vector3 position) : m_Model(model), m_Texture(texture),
                                                                   m_Position(position)
{
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

Blahaj::Blahaj(Model model, Texture2D texture, Vector3 position, Vector3 scale) : m_Model(model), m_Texture(texture),
    m_Position(position), m_Scale(scale)
{
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

Blahaj::Blahaj(Model model, Texture2D texture, Vector3 position, Vector3 scale, float rotationSpeed): m_Model(model),
    m_Texture(texture),
    m_Position(position), m_Scale(scale), m_RotationSpeed(rotationSpeed)
{
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

Blahaj::~Blahaj()
{
    TraceLog(LOG_INFO, "Unloading blahaj");

    UnloadTexture(m_Texture);
    UnloadModel(m_Model);
}

void Blahaj::Render() const
{
    DrawModelEx(m_Model, m_Position, m_RotationAxis, m_RotationAngle, m_Scale, WHITE);
}

void Blahaj::Rotate()
{
    const float newYAngle = m_RotationAxis.y + m_RotationSpeed * GetFrameTime();

    // Create quaternions for start and end positions
    const Quaternion start = QuaternionFromEuler(m_RotationAxis.z, m_RotationAxis.y,
                                                 m_RotationAxis.x);
    const Quaternion end = QuaternionFromEuler(0, newYAngle, 0); // Rotate around Y-axis

    // Slerp the quaternions to get the smooth transition
    const Quaternion slerp = QuaternionSlerp(start, end, m_RotationSpeed * GetFrameTime());

    // Apply the new rotation to the model
    const Matrix modelTransform = QuaternionToMatrix(slerp);
    m_Model.transform = modelTransform;

    // Update the model's rotation
    m_RotationAxis.y = newYAngle;
}

