#pragma once

#include "stdinclude.h"

class Device;
class Camera;
class Model;
class Pipeline;

class Entity {
public:
    Entity(Device* device, Camera* camera);
    ~Entity();
    virtual void Init();

    void SetFrontFace(VkFrontFace face);

    void LoadTexture(std::string filepath);

    void LoadModel(std::string filepath);

    void createTextureImage(std::string filepath);

    void createTextureImageView();

    void transitionImageLayout(
        VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void Draw(VkCommandBuffer commandBuffer, int i);

    Device* device;
    Camera* camera;

    Model* m_model;

    Pipeline* pipeline;

    VkImage textureImage = VK_NULL_HANDLE;
    VkDeviceMemory textureImageMemory;

    float direction, speed;

    glm::vec3 position;
};
