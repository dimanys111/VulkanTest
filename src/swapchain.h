#pragma once

#include "stdinclude.h"

class Device;
class WindowManager;

class SwapChain {
public:
    SwapChain(std::shared_ptr<WindowManager> window, std::shared_ptr<Device> device);

    ~SwapChain();

    void Init();

    VkSwapchainKHR& swapChain() { return m_swapChain; }

    std::vector<VkImageView>& swapChainImageViews() { return m_swapChainImageViews; }

private:
    void createSwapChain();

    void createImageViews();

    SwapChainSupportDetails querySwapChainSupport();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;

    std::shared_ptr<WindowManager> m_window;
    std::shared_ptr<Device> m_device;
};
