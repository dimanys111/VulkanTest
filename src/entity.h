#pragma once

#include "stdinclude.h"

class Device;
class Camera;
class Model;
class Pipeline;

class Entity {
public:
    Entity(std::shared_ptr<Device> device, std::shared_ptr<Camera> camera);
    virtual ~Entity();
    virtual void Init();
    void Draw(VkCommandBuffer commandBuffer, int i);

    std::shared_ptr<Pipeline>& pipeline() { return m_pipeline; }

protected:
    void SetFrontFace(VkFrontFace face);

    void LoadTexture(const std::string& filepath);

    void LoadModel(const std::string& filepath);

    void createTextureImage(const std::string& filepath);

    void createTextureImageView();

    void transitionImageLayout(
        VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    std::shared_ptr<Device> m_device;
    std::shared_ptr<Camera> m_camera;

    std::shared_ptr<Model> m_modelObj;

    std::shared_ptr<Pipeline> m_pipeline;

    VkImage m_textureImage = VK_NULL_HANDLE;
    VkDeviceMemory m_textureImageMemory;

    glm::vec3 m_position;
};
