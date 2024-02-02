#include "camera.h"
#include "resource.h"
#include "window.h"

Camera::Camera(float width, float height)
{
    mouseSensitivity = 0.f;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    proj = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, ViewDistance);
    proj[1][1] *= -1;

    yaw = -90.0f;
    pitch = 0.0f;
}

Camera::~Camera() { }

void Camera::processMouse(float xpos, float ypos)
{

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void Camera::Update(float deltaTime)
{

    const float cameraSpeed = 0.2f; // настройте по вашему усмотрению
    float currSpeed;

    if (Resource::pressed[GLFW_KEY_LEFT_SHIFT])
        currSpeed = cameraSpeed * 10;
    else
        currSpeed = cameraSpeed;

    if (Resource::pressed[GLFW_KEY_W])
        cameraPos += currSpeed * cameraFront * deltaTime;
    if (Resource::pressed[GLFW_KEY_S])
        cameraPos -= currSpeed * cameraFront * deltaTime;
    if (Resource::pressed[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * currSpeed * deltaTime;
    if (Resource::pressed[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * currSpeed * deltaTime;

    if (!Resource::showCursor)
        processMouse(WindowManager::xpos, WindowManager::ypos);

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::vec3 Camera::GetPosition() { return cameraPos * 10.f; }
