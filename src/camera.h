#pragma once

#include "stdinclude.h"

class Camera {
public:
    Camera(float width, float height);
    ~Camera();

    void processMouse(float xpos, float ypos);

    void Update(float deltaTime);

    glm::vec3 GetPosition();

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;

    inline static float ViewDistance = 600.f;

private:
    float yaw;
    float pitch;
    float mouseSensitivity;
    float lastX = 400, lastY = 300;
    bool firstMouse = true;
    glm::vec2 moveDir;
};
