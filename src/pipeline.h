#pragma once

#include "stdinclude.h"

#include "device.h"

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
};
