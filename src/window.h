#pragma once

#include "stdinclude.h"

class Device;

struct DestroyGlfwWin {
    void operator()(GLFWwindow* ptr) { glfwDestroyWindow(ptr); }
};

class WindowManager {
public:
    WindowManager();
    ~WindowManager();
    void Init();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    bool GetClose();
    void createSurface();

    std::shared_ptr<GLFWwindow> window;
    VkSurfaceKHR surface;
    VkInstance instance;
    inline static double xpos;
    inline static double ypos;
};
