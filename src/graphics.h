#pragma once

#include "stdinclude.h"

#include "resource.h"

#include "device.h"
#include "tools.h"
#include "window.h"
#include "swapchain.h"
#include "renderer.h"

#include "gameObject.h"

class Graphics{
public:
    Graphics(WindowManager *window, Device* device, SwapChain* swapchain);
    ~Graphics();

    void SetGameObject(GameObject* go);

    void Init();
    
    void createFramebuffers();

    void createDepthResources();


    void createCommandBuffers();

    void setCommandBuffers();

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkFramebuffer> swapChainFramebuffers;

    WindowManager* window;
    Device* device;
    SwapChain* swapchain;
    Renderer* renderer;

    std::vector<GameObject*> gameObjects;


    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    bool show = true;
};
