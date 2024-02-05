#pragma once

#include "stdinclude.h"

class Device;
class Camera;
class Model;
class Pipeline;

class Entity {
public:
    Entity(std::shared_ptr<Device> device, std::shared_ptr<Camera> camera);
    ~Entity();
    virtual void Init();

    void SetFrontFace(VkFrontFace face);

    void LoadTexture(const std::string& filepath);

    void LoadModel(const std::string& filepath);

    void createTextureImage(const std::string& filepath);

    void createTextureImageView();

    void transitionImageLayout(
        VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void Draw(VkCommandBuffer commandBuffer, int i);

    std::shared_ptr<Device> device;
    std::shared_ptr<Camera> camera;

    std::shared_ptr<Model> modelObj;

    std::shared_ptr<Pipeline> pipeline;

    VkImage textureImage = VK_NULL_HANDLE;
    VkDeviceMemory textureImageMemory;

    glm::vec3 position;
};
