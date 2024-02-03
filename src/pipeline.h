#pragma once

#include "stdinclude.h"

class Device;

class Pipeline {

public:
    Pipeline(Device* device);
    ~Pipeline();

    void Init();

    void createTextureSampler();

    void createDescriptorPool();

    void createDescriptorSetLayout();

    void createDescriptorSets();

    void createUniformBuffers();
    void createGraphicsPipeline(
        std::string vertFile, std::string fragFile, const VkRenderPass& renderPass);

    Device* device = NULL;

    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    VkImageView textureImageView = VK_NULL_HANDLE;
    VkSampler textureSampler;

    VkFrontFace face = VK_FRONT_FACE_CLOCKWISE;

    std::vector<VkDescriptorSet> descriptorSets;

private:
    VkShaderModule createShaderModule(const std::vector<char>& code);
};
