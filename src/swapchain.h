#pragma once

#include "device.h"
#include "stdinclude.h"
#include "window.h"

#include "resource.h"
#include "tools.h"

class SwapChain {
public:
    SwapChain(WindowManager* window, Device* device);

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

    WindowManager* window;
    Device* device;
};
