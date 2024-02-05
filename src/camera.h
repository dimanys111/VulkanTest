#pragma once

#include "stdinclude.h"

class Camera {
public:
    Camera(float width, float height);
    ~Camera();

    void processMouse(float xpos, float ypos);

    void Update(float deltaTime);

    glm::vec3 GetPosition();

    inline static float ViewDistance = 600.f;

    glm::mat4 view() const { return m_view; }

    glm::mat4 proj() const { return m_proj; }

private:
    float m_yaw;
    float m_pitch;
    float m_mouseSensitivity;
    float m_lastX = 400;
    float m_lastY = 300;
    bool m_firstMouse = true;
    glm::vec2 m_moveDir;
    glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_proj;
};
