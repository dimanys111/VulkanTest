#pragma once

#include "stdinclude.h"

class Device;

class Model {
public:
    Model(Device* device);
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

    Device* device;
};
