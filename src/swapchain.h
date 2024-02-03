#pragma once

#include "stdinclude.h"

class Device;
class WindowManager;

class SwapChain {
public:
    SwapChain(std::shared_ptr<WindowManager> window, std::shared_ptr<Device> device);

    ~SwapChain();

    void Init();

    void createSwapChain();

    void createImageViews();

    SwapChainSupportDetails querySwapChainSupport();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    std::shared_ptr<WindowManager> window;
    std::shared_ptr<Device> device;
};
