#include "model.h"
#include "device.h"
#include "tools.h"

Model::Model(std::shared_ptr<Device> device)
    : m_device(device)
{
}

Model::~Model()
{
    if (m_indices.size() > 0) {
        vkDestroyBuffer(m_device->device(), m_indexBuffer, nullptr);
        vkFreeMemory(m_device->device(), m_indexBufferMemory, nullptr);
    }

    if (m_vertices.size() > 0) {
        vkDestroyBuffer(m_device->device(), m_vertexBuffer, nullptr);
        vkFreeMemory(m_device->device(), m_vertexBufferMemory, nullptr);
    }
}

void Model::Init()
{
    createVertexBuffer();
    createIndexBuffer();
}

void Model::createVertexBuffer()
{
    if (m_vertices.size() == 0)
        return;

    VkDeviceSize bufferSize = sizeof(m_vertices[0]) * m_vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Tools::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
        stagingBufferMemory);

    void* data;
    vkMapMemory(m_device->device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, m_vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(m_device->device(), stagingBufferMemory);

    Tools::createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

    Tools::copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

    vkDestroyBuffer(m_device->device(), stagingBuffer, nullptr);
    vkFreeMemory(m_device->device(), stagingBufferMemory, nullptr);
}

void Model::createIndexBuffer()
{

    if (m_indices.size() == 0)
        return;

    VkDeviceSize bufferSize = sizeof(m_indices[0]) * m_indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Tools::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
        stagingBufferMemory);

    void* data;
    vkMapMemory(m_device->device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, m_indices.data(), (size_t)bufferSize);
    vkUnmapMemory(m_device->device(), stagingBufferMemory);

    Tools::createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

    Tools::copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

    vkDestroyBuffer(m_device->device(), stagingBuffer, nullptr);
    vkFreeMemory(m_device->device(), stagingBufferMemory, nullptr);
}
