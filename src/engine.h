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

    void Init();
    void Run();

    void GUIInit();

private:
    void Update();

    void draw();

    void FrameRender(uint32_t* imageIndex);

    void MakeFrame();

    void FramePresent(uint32_t imageIndex);

    void createSyncObjects();

    void createInstance();

    void setupDebugMessenger();

    bool checkValidationLayerSupport() const;

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const;

    VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator);

    void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    std::vector<const char*> getRequiredExtensions();

    std::shared_ptr<WindowManager> m_window;
    std::shared_ptr<Device> m_device;
    std::shared_ptr<SwapChain> m_swapchain;
    std::shared_ptr<Graphics> m_graphics;

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;

    std::vector<VkFence> m_inFlightFences;
    std::vector<VkFence> m_imagesInFlight;
    size_t m_currentFrame = 0;

    VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;

    bool m_show_demo_window = true;

    bool m_swapChainRebuild = false;

    std::shared_ptr<Game> m_game;

    float m_v[3];

    bool m_menuSwaped = false;

    bool m_show = true;

    VkDescriptorPool m_imguiPool;

    VkDebugUtilsMessengerEXT m_debugMessenger;
};
