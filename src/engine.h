#pragma once

#include "stdinclude.h"

class WindowManager;
class Device;
class SwapChain;
class Graphics;
class DeviceVkSemaphore;
class Game;

class Engine {
public:
    Engine();
    virtual ~Engine();

public:
    void Init();
    void Run();

    void GUIInit();

private:
    std::shared_ptr<WindowManager> window;
    std::shared_ptr<Device> device;
    std::shared_ptr<SwapChain> swapchain;
    std::shared_ptr<Graphics> graphics;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    void Update();

    void draw();

    void FrameRender(uint32_t* imageIndex);

    void MakeFrame();

    void FramePresent(uint32_t imageIndex);

    void createSyncObjects();

    void createInstance();

    void setupDebugMessenger();

    bool checkValidationLayerSupport();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator);

    void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    std::vector<const char*> getRequiredExtensions();

    VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;

    bool show_demo_window = true;

    bool g_SwapChainRebuild = false;

    std::shared_ptr<Game> game;

    float v[3];

    bool menuSwaped = false;

    bool show = true;

    VkDescriptorPool imguiPool;

    VkDebugUtilsMessengerEXT debugMessenger;
};
