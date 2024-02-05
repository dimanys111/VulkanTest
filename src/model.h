#pragma once

#include "stdinclude.h"

class Device;

class Model {
public:
    explicit Model(std::shared_ptr<Device> device);
    ~Model();
    void Init();

    std::vector<uint16_t>& indices() { return m_indices; }

    std::vector<Vertex>& vertices() { return m_vertices; }

    VkBuffer& indexBuffer() { return m_indexBuffer; }

    VkBuffer& vertexBuffer() { return m_vertexBuffer; }

private:
    void createVertexBuffer();

    void createIndexBuffer();

    std::vector<Vertex> m_vertices;

    std::vector<uint16_t> m_indices;

    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;

    std::shared_ptr<Device> m_device;
};
