#pragma once

#include "stdinclude.h"

class WindowManager;

class Device {
public:
    explicit Device(std::shared_ptr<WindowManager> window);

    ~Device();

    void Init();

    VkDevice device() const { return m_device; }

    VkQueue graphicsQueue() const { return m_graphicsQueue; }

    VkQueue presentQueue() const { return m_presentQueue; }

    uint32_t queueFamily() const { return m_queueFamily; }

    VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    const inline static std::vector<const char*> validationLayers
        = { "VK_LAYER_KHRONOS_validation" };
    const inline static std::vector<const char*> deviceExtensions
        = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

private:
    void pickPhysicalDevice();

    void createLogicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;

    uint32_t m_queueFamily;

    std::shared_ptr<WindowManager> m_window;
};
