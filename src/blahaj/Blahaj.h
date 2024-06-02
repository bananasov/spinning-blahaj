#pragma once
#include <raylib.h>

class Blahaj {
public:
    Blahaj(Model model, Texture2D texture);
    Blahaj(Model model, Texture2D texture, Vector3 position);
    Blahaj(Model model, Texture2D texture, Vector3 position, Vector3 scale);
    Blahaj(Model model, Texture2D texture, Vector3 position, Vector3 scale, float rotationSpeed);
    ~Blahaj();
public:
    void Render() const;
    void Rotate();

    [[nodiscard]] float GetRotationSpeed() const { return m_RotationSpeed; }
    [[nodiscard]] Vector3 GetScale() const { return m_Scale; }

    void SetRotationSpeed(const float speed) { m_RotationSpeed = speed; }
    void SetScale(const Vector3 scale) { m_Scale = scale; }
private:
    Model m_Model;
    Texture2D m_Texture;

    Vector3 m_Position = { 0.0f, 0.0f, 0.0f };
    Vector3 m_Scale = { 10.0f, 10.f, 10.f };
    Vector3 m_RotationAxis = { 0.0f, 0.0f, 0.0f };

    float m_RotationAngle = 0.0f;
    float m_RotationSpeed = 1.0f;
};

