#pragma once

#include "stdinclude.h"

class Device;
class Pipeline;

class Renderer {
public:
    explicit Renderer(std::shared_ptr<Device> device);
    ~Renderer();
    VkRenderPass& renderPass() { return m_renderPass; }
    VkFormat findDepthFormat();

private:
    void createCommandPool();

    void createRenderPass();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
        VkFormatFeatureFlags features);

    VkRenderPass m_renderPass;

    std::shared_ptr<Device> m_device;
};
