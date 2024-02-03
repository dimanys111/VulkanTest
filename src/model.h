#pragma once

#include "stdinclude.h"

class Device;

class Model {
public:
    explicit Model(std::shared_ptr<Device> device);
    ~Model();
    void Init();

    void createVertexBuffer();

    void createIndexBuffer();

    std::vector<Vertex> vertices;

    std::vector<uint16_t> indices;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    std::shared_ptr<Device> device;
};
