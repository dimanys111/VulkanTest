#pragma once

#include "stdinclude.h"

class Device;
class GameObject;
class Renderer;
class SwapChain;
class WindowManager;

class Graphics {
public:
    Graphics(std::shared_ptr<WindowManager> window, std::shared_ptr<Device> device,
        std::shared_ptr<SwapChain> swapchain);
    ~Graphics();

    void SetGameObject(std::shared_ptr<GameObject> go);

    void Init();

    void createFramebuffers();

    void createDepthResources();

    void createCommandBuffers();

    void setCommandBuffers();

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkFramebuffer> swapChainFramebuffers;

    std::shared_ptr<WindowManager> window;
    std::shared_ptr<Device> device;
    std::shared_ptr<SwapChain> swapchain;
    std::shared_ptr<Renderer> renderer;

    std::vector<std::shared_ptr<GameObject>> gameObjects;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    bool show = true;
};
