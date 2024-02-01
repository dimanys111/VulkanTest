#pragma once

#include "stdinclude.h"

#include "resource.h"

class WindowManager {
public:
    WindowManager();
    ~WindowManager();
    void Init();

    void createInstance();

    void createSurface();

    void setupDebugMessenger();

    VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator);

    void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    bool checkValidationLayerSupport();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    bool GetClose();

    std::vector<const char*> getRequiredExtensions();

    GLFWwindow* window;

    VkInstance instance;

    VkSurfaceKHR surface;

    VkDebugUtilsMessengerEXT debugMessenger;

    inline static double xpos;
    inline static double ypos;
};
