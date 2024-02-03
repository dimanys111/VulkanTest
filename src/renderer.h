#pragma once

#include "stdinclude.h"

class Device;
class Pipeline;

class Renderer {
public:
    explicit Renderer(std::shared_ptr<Device> device);
    ~Renderer();

    void createCommandPool();

    void createRenderPass();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
        VkFormatFeatureFlags features);

    VkFormat findDepthFormat();

    VkRenderPass renderPass;

    std::shared_ptr<Device> device;
};
