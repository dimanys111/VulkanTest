#include "window.h"
#include "resource.h"

WindowManager::WindowManager() { }

WindowManager::~WindowManager()
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window.get());

    glfwTerminate();
}

void WindowManager::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = std::shared_ptr<GLFWwindow>(
        glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr), DestroyGlfwWin());
    glfwSetWindowUserPointer(window.get(), this);
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window.get(), key_callback);
    glfwSetCursorPosCallback(window.get(), mouse_callback);
}

void WindowManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UNKNOWN)
        return; // Don't accept unknown keys

    if (action == GLFW_PRESS)
        Resource::pressed[key] = true;
    else if (action == GLFW_RELEASE)
        Resource::pressed[key] = false;

    // if (key == GLFW_KEY_E && action == GLFW_PRESS)
    //     activate_airship();
}

void WindowManager::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    WindowManager::xpos = xpos;
    WindowManager::ypos = ypos;
}

bool WindowManager::GetClose() { return glfwWindowShouldClose(window.get()); }

void WindowManager::createSurface()
{
    if (glfwCreateWindowSurface(instance, window.get(), nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}
