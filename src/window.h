#pragma once

#include "stdinclude.h"

class Device;

struct DestroyGlfwWin {
    void operator()(GLFWwindow* ptr) const { glfwDestroyWindow(ptr); }
};

class WindowManager {
public:
    WindowManager();
    ~WindowManager();
    void Init();

    VkSurfaceKHR& surface() { return m_surface; }
    VkInstance& instance() { return m_instance; }
    void createSurface();
    bool GetClose();

    GLFWwindow* window() { return m_window.get(); }
    inline static float m_xPos;
    inline static float m_yPos;

private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    std::shared_ptr<GLFWwindow> m_window;
    VkSurfaceKHR m_surface;
    VkInstance m_instance;
};
