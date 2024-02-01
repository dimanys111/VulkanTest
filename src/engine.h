#pragma once

#include "device.h"
#include "graphics.h"
#include "stdinclude.h"
#include "swapchain.h"
#include "window.h"

#include "game.h"

#include "resource.h"

class Engine {
public:
    Engine();
    virtual ~Engine();

public:
    void Init();
    void Run();

    void GUIInit();

private:
    WindowManager* window;
    Device* device;
    SwapChain* swapchain;
    Graphics* graphics;

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

    void drawFrame();

    void createSyncObjects();

    VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;

    bool show_demo_window = true;

    bool g_SwapChainRebuild = false;

    Game* game;

    float v[3];
    float p[3];

    bool menuSwaped = false;

    bool show = true, show_another_window;

    float clear_color;

    VkDescriptorPool imguiPool;
};
