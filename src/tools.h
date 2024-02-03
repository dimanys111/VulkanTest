#pragma once

#include "stdinclude.h"

class Device;

struct PrimitiveObject {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
};

enum PrimitiveType {
    PRIMITIVE_TYPE_SPHERE,
    PRIMITIVE_TYPE_CILINDER,
    PRIMITIVE_TYPE_CUBE,
    PRIMITIVE_TYPE_CAPSULE,
    PRIMITIVE_TYPE_QUAD
};

class Tools {
public:
    Tools(Device* device);
    ~Tools();

    static VkImageView createImageView(
        VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    static void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
        VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
        VkDeviceMemory& imageMemory);

    static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    static VkCommandBuffer beginSingleTimeCommands();

    static void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    static PrimitiveObject GetPrimitives(PrimitiveType type);

    static PrimitiveObject GetMCubes(glm::vec3 pos);

    inline static Device* device;

    static std::vector<char> readFile(const std::string& filename);

private:
    static PrimitiveObject CreateSphere();

    static PrimitiveObject GetUnitForCilinder(int sectCount);

    static PrimitiveObject CreatePipe();

    static PrimitiveObject CreateCapsule();

    static PrimitiveObject MakeMCubes(size_t size, glm::vec3 pos);

    // для удобства и избежания лишних расчетов и выделения памяти;)
    static double GetVal(int x, int y, int z, int i);

    static glm::vec3 GetPos(int x, int y, int z, int i);

    static void Polygonise(int x, int y, int z, double isolevel, PrimitiveObject* pObject);

    // Просто линейная интерполяция, наверное не очень быстрая, но узкое место не
    // здесь
    static glm::vec3 VertexInterp(
        double isolevel, glm::vec3 p1, glm::vec3 p2, double valp1, double valp2);

    static PrimitiveObject quad;

    static PrimitiveObject box;

    static std::vector<int> _edgeTable;

    static std::vector<std::vector<int>> _triTable;

    inline static std::vector<std::vector<std::vector<int>>> Gdata;
};
