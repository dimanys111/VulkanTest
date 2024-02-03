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

    VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;

    bool show_demo_window = true;

    bool g_SwapChainRebuild = false;

    std::shared_ptr<Game> game;

    float v[3];

    bool menuSwaped = false;

    bool show = true;

    VkDescriptorPool imguiPool;
};
