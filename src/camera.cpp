#include "camera.h"
#include "resource.h"
#include "window.h"

Camera::Camera(float width, float height)
{
    m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    m_proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, ViewDistance);
    m_proj[1][1] *= -1;
}

Camera::~Camera() = default;

void Camera::processMouse(float xpos, float ypos)
{

    if (m_firstMouse) {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
    }

    float xoffset = xpos - m_lastX;
    float yoffset = m_lastY - ypos;
    m_lastX = xpos;
    m_lastY = ypos;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cosf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));
    direction.y = sinf(glm::radians(m_pitch));
    direction.z = sinf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));
    m_cameraFront = glm::normalize(direction);
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
        m_cameraPos += currSpeed * m_cameraFront * deltaTime;
    if (Resource::pressed[GLFW_KEY_S])
        m_cameraPos -= currSpeed * m_cameraFront * deltaTime;
    if (Resource::pressed[GLFW_KEY_A])
        m_cameraPos
            -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * currSpeed * deltaTime;
    if (Resource::pressed[GLFW_KEY_D])
        m_cameraPos
            += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * currSpeed * deltaTime;

    if (!Resource::showCursor)
        processMouse(WindowManager::m_xPos, WindowManager::m_yPos);

    m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

glm::vec3 Camera::GetPosition() const { return m_cameraPos * 10.f; }
