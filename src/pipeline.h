#pragma once

#include "stdinclude.h"

class Device;

class Pipeline {

public:
    explicit Pipeline(std::shared_ptr<Device> device);
    ~Pipeline();

    void Init();

    std::vector<VkDescriptorSet>& descriptorSets() { return m_descriptorSets; }

    VkPipelineLayout& pipelineLayout() { return m_pipelineLayout; }

    VkPipeline& graphicsPipeline() { return m_graphicsPipeline; }

    VkImageView& textureImageView() { return m_textureImageView; }

    void setTextureImageView(const VkImageView& textureImageView)
    {
        m_textureImageView = textureImageView;
    }

    VkFrontFace& face() { return m_face; }

    void setFace(const VkFrontFace& face) { m_face = face; }
    void createGraphicsPipeline(
        const std::string& vertFile, const std::string& fragFile, const VkRenderPass& renderPass);

    std::vector<VkDeviceMemory>& uniformBuffersMemory() { return m_uniformBuffersMemory; }

private:
    void createTextureSampler();

    void createDescriptorPool();

    void createDescriptorSetLayout();

    void createDescriptorSets();

    VkShaderModule createShaderModule(const std::vector<char>& code);

    void createUniformBuffers();

    std::shared_ptr<Device> m_device;

    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    VkDescriptorSetLayout m_descriptorSetLayout;
    VkDescriptorPool m_descriptorPool;

    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;

    VkImageView m_textureImageView = VK_NULL_HANDLE;
    VkSampler m_textureSampler;

    VkFrontFace m_face = VK_FRONT_FACE_CLOCKWISE;

    std::vector<VkDescriptorSet> m_descriptorSets;
};
