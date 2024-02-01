#pragma once

#include "stdinclude.h"

#include "device.h"

#include "resource.h"

#include "pipeline.h"

class Renderer {
public:
    Renderer(Device* device);
    ~Renderer();

    void createCommandPool();

    void createRenderPass();

    void createGraphicsPipeline(std::string vertFile, std::string fragFile, Pipeline* pipeline);

    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
        VkFormatFeatureFlags features);

    VkFormat findDepthFormat();

    static std::vector<char> readFile(const std::string& filename);

    VkRenderPass renderPass;

    Device* device;
};
