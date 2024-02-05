#include "swapchain.h"
#include "device.h"
#include "resource.h"
#include "tools.h"
#include "window.h"

SwapChain::SwapChain(std::shared_ptr<WindowManager> window, std::shared_ptr<Device> device)
{
    m_window = window;
    m_device = device;
}

SwapChain::~SwapChain()
{
    for (auto imageView : m_swapChainImageViews) {
        vkDestroyImageView(m_device->device(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(m_device->device(), m_swapChain, nullptr);
}

void SwapChain::Init()
{
    createSwapChain();
    createImageViews();
}

void SwapChain::createSwapChain()
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport();

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0
        && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_window->surface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = m_device->findQueueFamilies(m_device->physicalDevice());
    uint32_t queueFamilyIndices[]
        = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_device->device(), &createInfo, nullptr, &m_swapChain)
        != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(m_device->device(), m_swapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device->device(), m_swapChain, &imageCount, m_swapChainImages.data());

    Resource::swapChainImageFormat = surfaceFormat.format;
    Resource::swapChainExtent = extent;
    Resource::countFrames = imageCount;
}

void SwapChain::createImageViews()
{
    m_swapChainImageViews.resize(Resource::countFrames);

    for (size_t i = 0; i < Resource::countFrames; i++) {

        m_swapChainImageViews[i] = Tools::createImageView(
            m_swapChainImages[i], Resource::swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

SwapChainSupportDetails SwapChain::querySwapChainSupport()
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        m_device->physicalDevice(), m_window->surface(), &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        m_device->physicalDevice(), m_window->surface(), &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(
            m_device->physicalDevice(), m_window->surface(), &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        m_device->physicalDevice(), m_window->surface(), &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_device->physicalDevice(), m_window->surface(),
            &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    if (auto it = std::find_if(availableFormats.cbegin(), availableFormats.cend(),
            [](const auto& availableFormat) {
                return availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
                    && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
            });
        it != availableFormats.cend()) {
        return *it;
    }

    return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    if (auto it = std::find_if(availablePresentModes.cbegin(), availablePresentModes.cend(),
            [](const auto& availablePresentMode) {
                return availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR;
            });
        it != availablePresentModes.cend()) {
        return *it;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(m_window->window(), &width, &height);

        VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
