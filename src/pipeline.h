#pragma once

#include "stdinclude.h"

class Device;

class Pipeline {

public:
    explicit Pipeline(std::shared_ptr<Device> device);
    ~Pipeline();

    void Init();

    void createTextureSampler();

    void createDescriptorPool();

    void createDescriptorSetLayout();

    void createDescriptorSets();

    void createUniformBuffers();
    void createGraphicsPipeline(
        const std::string& vertFile, const std::string& fragFile, const VkRenderPass& renderPass);

    std::shared_ptr<Device> device;

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
