#include "window.h"
#include "resource.h"

WindowManager::WindowManager() = default;

WindowManager::~WindowManager()
{
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkDestroyInstance(m_instance, nullptr);

    glfwDestroyWindow(m_window.get());

    glfwTerminate();
}

void WindowManager::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = std::shared_ptr<GLFWwindow>(
        glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr), DestroyGlfwWin());
    glfwSetWindowUserPointer(m_window.get(), this);
    // glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(m_window.get(), key_callback);
    glfwSetCursorPosCallback(m_window.get(), mouse_callback);
}

void WindowManager::key_callback([[maybe_unused]] GLFWwindow* window, int key,
    [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods)
{
    if (key == GLFW_KEY_UNKNOWN)
        return; // Don't accept unknown keys

    if (action == GLFW_PRESS)
        Resource::pressed[key] = true;
    else if (action == GLFW_RELEASE)
        Resource::pressed[key] = false;
}

void WindowManager::mouse_callback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos)
{
    m_xPos = xpos;
    m_yPos = ypos;
}

bool WindowManager::GetClose() const { return glfwWindowShouldClose(m_window.get()); }

void WindowManager::createSurface()
{
    if (glfwCreateWindowSurface(m_instance, m_window.get(), nullptr, &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}
