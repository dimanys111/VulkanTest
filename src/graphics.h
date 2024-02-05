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

    std::vector<VkCommandBuffer>& commandBuffers() { return m_commandBuffers; }

    void setCommandBuffers();

    std::shared_ptr<Renderer>& renderer() { return m_renderer; }

    std::vector<VkFramebuffer>& swapChainFramebuffers() { return m_swapChainFramebuffers; }

private:
    void createFramebuffers();

    void createDepthResources();

    void createCommandBuffers();

    std::vector<VkCommandBuffer> m_commandBuffers;

    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    std::shared_ptr<WindowManager> m_window;
    std::shared_ptr<Device> m_device;
    std::shared_ptr<SwapChain> m_swapchain;
    std::shared_ptr<Renderer> m_renderer;

    std::vector<std::shared_ptr<GameObject>> m_gameObjects;

    VkImage m_depthImage;
    VkDeviceMemory m_depthImageMemory;
    VkImageView m_depthImageView;

    bool m_show = true;
};
