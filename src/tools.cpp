#include "tools.h"
#include "SimplexNoise.h"
#include "device.h"
#include "resource.h"

Tools::Tools(Device* device) { this->device = device; }

Tools::~Tools() { }

VkImageView Tools::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device->device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

void Tools::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
    VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
    VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(device->device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device->device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device->findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device->device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(device->device, image, imageMemory, 0);
}

void Tools::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device->device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device->device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device->findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device->device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device->device, buffer, bufferMemory, 0);
}

void Tools::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

void Tools::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(
        commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer Tools::beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Resource::commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device->device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Tools::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->graphicsQueue);

    vkFreeCommandBuffers(device->device, Resource::commandPool, 1, &commandBuffer);
}

PrimitiveObject Tools::GetPrimitives(PrimitiveType type)
{
    switch (type) {
    case PrimitiveType::PRIMITIVE_TYPE_SPHERE:
        return CreateSphere();
    case PrimitiveType::PRIMITIVE_TYPE_CILINDER:
        return CreatePipe();
    case PrimitiveType::PRIMITIVE_TYPE_CUBE:
        return box;
    case PrimitiveType::PRIMITIVE_TYPE_CAPSULE:
        return CreateCapsule();
    case PrimitiveType::PRIMITIVE_TYPE_QUAD:
        return quad;
    }

    return PrimitiveObject({ {}, {} });
}

PrimitiveObject Tools::GetMCubes(glm::vec3 pos) { return MakeMCubes(16, pos); }

PrimitiveObject Tools::CreateSphere()
{
    PrimitiveObject pObject;

    float x, y, z, xy;
    float radius = 0.2f; // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius; // vertex normal
    float s, t; // vertex texCoord

    int stackCount = 20;
    int sectorCount = 20;
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle); // r * cos(u)
        z = radius * sinf(stackAngle); // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different
        // tex coords
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            pObject.vertices.push_back({ { x, y, z }, { s, t, t } });
        }
    }

    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1); // beginning of current stack
        k2 = k1 + sectorCount + 1; // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0) {
                pObject.indices.push_back(k1 + 1);
                pObject.indices.push_back(k2);
                pObject.indices.push_back(k1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1)) {
                pObject.indices.push_back(k2 + 1);
                pObject.indices.push_back(k2);
                pObject.indices.push_back(k1 + 1);
            }
        }
    }

    return pObject;
}

PrimitiveObject Tools::GetUnitForCilinder(int sectCount)
{
    PrimitiveObject pObject;

    float sectorStep = 2 * M_PI / sectCount;
    float sectorAngle; // radian

    std::vector<float> unitCircleVertices;
    for (int i = 0; i <= sectCount; ++i) {
        sectorAngle = i * sectorStep;
        pObject.vertices.push_back({ { cos(sectorAngle), sin(sectorAngle), 0 },
            { cos(sectorAngle), sin(sectorAngle), 1.0f } });
    }

    return pObject;
}

PrimitiveObject Tools::CreatePipe()
{

    PrimitiveObject pObject;

    float radius = 1;
    float height = 3;
    int sectorCount = 10;

    pObject = GetUnitForCilinder(sectorCount);

    // put side vertices to arrays
    for (int i = 0; i < 2; ++i) {
        float h = -height + i * height; // z value; -h/2 to h/2
        float t = 1.0f - i; // vertical tex coord; 1 to 0

        for (int j = 0, k = 0; j <= sectorCount; ++j, k++) {
            float ux = pObject.vertices[k].pos.x;
            float uy = pObject.vertices[k].pos.y;
            float uz = pObject.vertices[k].pos.z;

            pObject.vertices.push_back(
                { { ux * radius, uy * radius, h }, { (float)j / sectorCount, t, t } });
        }
    }

    // the starting index for the base/top surface
    // NOTE: it is used for generating indices later
    int baseCenterIndex = pObject.vertices.size();
    int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

    // put base and top vertices to arrays
    for (int i = 0; i < 2; ++i) {
        float h = -height + i * height; // z value; -h/2 to h/2
        float nz = -1 + i * 2; // z value of normal; -1 to 1

        pObject.vertices.push_back({ { 0, 0, h }, { 0.5f, 0.5f, 0.5f } });

        for (int j = 0, k = 0; j < sectorCount; ++j, k++) {
            float ux = pObject.vertices[k].pos.x;
            float uy = pObject.vertices[k].pos.y;
            pObject.vertices.push_back({ { ux * radius, uy * radius, h },
                { -ux * 0.5f + 0.5f, -uy * 0.5f + 0.5f, -uy * 0.5f + 0.5f } });
        }
    }

    int k1 = 0; // 1st vertex index at base
    int k2 = sectorCount + 1; // 1st vertex index at top

    // indices for the side surface
    for (int i = 0; i < sectorCount + 1; ++i, ++k1, ++k2) {
        // 2 triangles per sector
        // k1 => k1+1 => k2
        pObject.indices.push_back(k1);
        pObject.indices.push_back(k1 + 1);
        pObject.indices.push_back(k2);

        // k2 => k1+1 => k2+1
        pObject.indices.push_back(k2);
        pObject.indices.push_back(k1 + 1);
        pObject.indices.push_back(k2 + 1);
    }

    // indices for the base surface
    // NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex
    // generation
    //      please see the previous code snippet
    for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k) {
        if (i < sectorCount - 1) {
            pObject.indices.push_back(k);
            pObject.indices.push_back(k + 1);
            pObject.indices.push_back(baseCenterIndex);
        } else // last triangle
        {
            pObject.indices.push_back(k);
            pObject.indices.push_back(baseCenterIndex + 1);
            pObject.indices.push_back(baseCenterIndex);
        }
    }

    // indices for the top surface
    for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k) {
        if (i < sectorCount - 1) {
            pObject.indices.push_back(k + 1);
            pObject.indices.push_back(k);
            pObject.indices.push_back(topCenterIndex);
        } else // last triangle
        {
            pObject.indices.push_back(topCenterIndex + 1);
            pObject.indices.push_back(k);
            pObject.indices.push_back(topCenterIndex);
        }
    }

    return pObject;
}

PrimitiveObject Tools::CreateCapsule()
{
    PrimitiveObject pObject;

    float x, y, z, xy;
    float radius = 0.5f;
    float height = 0.5f; // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius; // vertex normal
    float s, t; // vertex texCoord

    int stackCount = 10;
    int sectorCount = 10;
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount / 2 - 1; ++i) {
        stackAngle = M_PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle); // r * cos(u)
        z = radius * sinf(stackAngle); // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different
        // tex coords
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            pObject.vertices.push_back({ { x, y, z }, { 1.0f, 1.0f, 1.0f }, { s, t, t } });
        }
    }

    int indexTop, indexBase;
    indexTop = pObject.vertices.size() - sectorCount - 1; // 1st vertex index at top
    // put side vertices to arrays
    for (int i = 0; i < 2; ++i) {
        float h = -height + i * height; // z value; -h/2 to h/2
        float t = 1.0f - i; // vertical tex coord; 1 to 0

        for (int j = 0, k = 0; j <= sectorCount; ++j, k++) {
            float ux = pObject.vertices[pObject.vertices.size() - 1 - k].pos.x;
            float uy = pObject.vertices[pObject.vertices.size() - 1 - k].pos.y;
            float uz = pObject.vertices[pObject.vertices.size() - 1 - k].pos.z;

            pObject.vertices.push_back({ { ux * radius * 10, uy * radius * 10, h },
                { 1.0f, 1.0f, 1.0f }, { (float)j / sectorCount, t, t } });
        }
    }

    indexBase = pObject.vertices.size(); // 1st vertex index at base

    for (int i = stackCount / 2 + 1; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle); // r * cos(u)
        z = radius * sinf(stackAngle); // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different
        // tex coords
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep; // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            pObject.vertices.push_back({ { x, y, z - height }, { 1.0f, 1.0f, 1.0f }, { s, t, t } });
        }
    }

    //---------------------------------------------Index
    int k1, k2;
    for (int i = 0; i < stackCount / 2 - 1; ++i) {
        k1 = i * (sectorCount + 1); // beginning of current stack
        k2 = k1 + sectorCount + 1; // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0) {
                pObject.indices.push_back(k1 + 1);
                pObject.indices.push_back(k2);
                pObject.indices.push_back(k1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount / 2 - 1)) {
                pObject.indices.push_back(k2 + 1);
                pObject.indices.push_back(k2);
                pObject.indices.push_back(k1 + 1);
            }
        }
    }

    // indices for the side surface
    for (int i = 0; i < sectorCount; ++i, ++indexTop, ++indexBase) {
        // 2 triangles per sector
        // k1 => k1+1 => k2
        pObject.indices.push_back(indexTop);
        pObject.indices.push_back(indexTop + 1);
        pObject.indices.push_back(indexBase);

        // k2 => k1+1 => k2+1
        pObject.indices.push_back(indexBase);
        pObject.indices.push_back(indexTop + 1);
        pObject.indices.push_back(indexBase + 1);
    }

    int end = (stackCount / 2 * (sectorCount)) + indexBase - 1;

    for (int i = indexBase - sectorCount - 1; i < end; ++i) {
        k1 = i; // beginning of current stack
        k2 = k1 + sectorCount + 1; // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != indexBase - 1) {
                pObject.indices.push_back(k1 + 1);
                pObject.indices.push_back(k2);
                pObject.indices.push_back(k1);
            }

            // k1+1 => k2 => k2+1
            if (i != (end - 1)) {
                pObject.indices.push_back(k2 + 1);
                pObject.indices.push_back(k2);
                pObject.indices.push_back(k1 + 1);
            }
        }
    }

    return pObject;
}

PrimitiveObject Tools::MakeMCubes(size_t size, glm::vec3 pos)
{

    PrimitiveObject pObject;

    BasicPerlinNoise noise;

    Gdata.clear();

    double fx = size / 8.0f;
    double fy = size / 8.0f;

    srand(234525);

    Gdata.resize(size + 1);
    for (int x = 0; x < size + 1; x++) {
        Gdata[x].resize(size + 1);
        for (int y = 0; y < size + 1; y++) {
            Gdata[x][y].resize(size + 1);
            for (int z = 0; z < size + 1; z++) {
                float val = 0;
                float posx = (pos.x * 16) + x;
                float posz = (pos.z * 16) + z;
                // if(y > size / 2)
                {
                    // val = std::sin((posx) / 50 + (posz) / 50) * 10;
                    val = noise.accumulatedOctaveNoise2D(posx / 100, posz / 100, 8.0f) * 10 + 20;
                    if (val > (pos.y * 16) + y)
                        Gdata[x][y][z] = val;
                    else
                        Gdata[x][y][z] = 0;
                }
                // else if( y <= size / 2)
                // {
                //     val = noise.noise(x, y, z);
                //     Gdata[x][y][z] = (1 + val) - y ;
                // }
            }
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                // Заполняем _triangles и получаем количество треугольников
                Polygonise(i, j, k, 5, &pObject);
            }
        }
    }

    return pObject;
}

double Tools::GetVal(int x, int y, int z, int i)
{
    switch (i) {
    case 0:
        return Gdata[x][y][z];
    case 1:
        return Gdata[x + 1][y][z];
    case 2:
        return Gdata[x + 1][y + 1][z];
    case 3:
        return Gdata[x][y + 1][z];
    case 4:
        return Gdata[x][y][z + 1];
    case 5:
        return Gdata[x + 1][y][z + 1];
    case 6:
        return Gdata[x + 1][y + 1][z + 1];
    case 7:
        return Gdata[x][y + 1][z + 1];
    }
    return 0;
}

glm::vec3 Tools::GetPos(int x, int y, int z, int i)
{
    glm::vec3 nVec;
    switch (i) {
    case 0:
        nVec.x = x;
        nVec.y = y;
        nVec.z = z;
        return nVec;
    case 1:
        nVec.x = x + 1;
        nVec.y = y;
        nVec.z = z;
        return nVec;
    case 2:
        nVec.x = x + 1;
        nVec.y = y + 1;
        nVec.z = z;
        return nVec;
    case 3:
        nVec.x = x;
        nVec.y = y + 1;
        nVec.z = z;
        return nVec;
    case 4:
        nVec.x = x;
        nVec.y = y;
        nVec.z = z + 1;
        return nVec;
    case 5:
        nVec.x = x + 1;
        nVec.y = y;
        nVec.z = z + 1;
        return nVec;
    case 6:
        nVec.x = x + 1;
        nVec.y = y + 1;
        nVec.z = z + 1;
        return nVec;
    case 7:
        nVec.x = x;
        nVec.y = y + 1;
        nVec.z = z + 1;
        return nVec;
    }
    return nVec;
}

void Tools::Polygonise(int x, int y, int z, double isolevel, PrimitiveObject* pObject)
{
    glm::vec3* vertlist = new glm::vec3[12];
    int i, ntriang = 0;
    int cubeindex = 0;

    // Определяем какой куб перед нами(фактически индекс в таблице)
    if (GetVal(x, y, z, 0) > isolevel)
        cubeindex |= 1;
    if (GetVal(x, y, z, 1) > isolevel)
        cubeindex |= 2;
    if (GetVal(x, y, z, 2) > isolevel)
        cubeindex |= 4;
    if (GetVal(x, y, z, 3) > isolevel)
        cubeindex |= 8;
    if (GetVal(x, y, z, 4) > isolevel)
        cubeindex |= 16;
    if (GetVal(x, y, z, 5) > isolevel)
        cubeindex |= 32;
    if (GetVal(x, y, z, 6) > isolevel)
        cubeindex |= 64;
    if (GetVal(x, y, z, 7) > isolevel)
        cubeindex |= 128;

    /* Cube is entirely in/out of the surface */
    if (_edgeTable[cubeindex] == 0)
        return;

    // Ищем конкретные положения вершин, используя линейную интерполяцию
    /* Find the vertices where the surface intersects the cube */
    if ((_edgeTable[cubeindex] & 1) > 0)
        vertlist[0] = VertexInterp(isolevel, GetPos(x, y, z, 0), GetPos(x, y, z, 1),
            GetVal(x, y, z, 0), GetVal(x, y, z, 1));
    if ((_edgeTable[cubeindex] & 2) > 0)
        vertlist[1] = VertexInterp(isolevel, GetPos(x, y, z, 1), GetPos(x, y, z, 2),
            GetVal(x, y, z, 1), GetVal(x, y, z, 2));
    if ((_edgeTable[cubeindex] & 4) > 0)
        vertlist[2] = VertexInterp(isolevel, GetPos(x, y, z, 2), GetPos(x, y, z, 3),
            GetVal(x, y, z, 2), GetVal(x, y, z, 3));
    if ((_edgeTable[cubeindex] & 8) > 0)
        vertlist[3] = VertexInterp(isolevel, GetPos(x, y, z, 3), GetPos(x, y, z, 0),
            GetVal(x, y, z, 3), GetVal(x, y, z, 0));
    if ((_edgeTable[cubeindex] & 16) > 0)
        vertlist[4] = VertexInterp(isolevel, GetPos(x, y, z, 4), GetPos(x, y, z, 5),
            GetVal(x, y, z, 4), GetVal(x, y, z, 5));
    if ((_edgeTable[cubeindex] & 32) > 0)
        vertlist[5] = VertexInterp(isolevel, GetPos(x, y, z, 5), GetPos(x, y, z, 6),
            GetVal(x, y, z, 5), GetVal(x, y, z, 6));
    if ((_edgeTable[cubeindex] & 64) > 0)
        vertlist[6] = VertexInterp(isolevel, GetPos(x, y, z, 6), GetPos(x, y, z, 7),
            GetVal(x, y, z, 6), GetVal(x, y, z, 7));
    if ((_edgeTable[cubeindex] & 128) > 0)
        vertlist[7] = VertexInterp(isolevel, GetPos(x, y, z, 7), GetPos(x, y, z, 4),
            GetVal(x, y, z, 7), GetVal(x, y, z, 4));
    if ((_edgeTable[cubeindex] & 256) > 0)
        vertlist[8] = VertexInterp(isolevel, GetPos(x, y, z, 0), GetPos(x, y, z, 4),
            GetVal(x, y, z, 0), GetVal(x, y, z, 4));
    if ((_edgeTable[cubeindex] & 512) > 0)
        vertlist[9] = VertexInterp(isolevel, GetPos(x, y, z, 1), GetPos(x, y, z, 5),
            GetVal(x, y, z, 1), GetVal(x, y, z, 5));
    if ((_edgeTable[cubeindex] & 1024) > 0)
        vertlist[10] = VertexInterp(isolevel, GetPos(x, y, z, 2), GetPos(x, y, z, 6),
            GetVal(x, y, z, 2), GetVal(x, y, z, 6));
    if ((_edgeTable[cubeindex] & 2048) > 0)
        vertlist[11] = VertexInterp(isolevel, GetPos(x, y, z, 3), GetPos(x, y, z, 7),
            GetVal(x, y, z, 3), GetVal(x, y, z, 7));

    // Ну и создаем треугольник, индексы вершин берем из _triTable, а вершины
    // определяем по cubeindex
    /* Create the triangle */
    for (i = 0; _triTable[cubeindex][i] != -1; i += 3) {
        float r = 0.2f;
        float g = 0.6f;
        float b = 0.2f;

        glm::vec3 normal
            = glm::cross(vertlist[_triTable[cubeindex][i + 1]] - vertlist[_triTable[cubeindex][i]],
                vertlist[_triTable[cubeindex][i + 2]] - vertlist[_triTable[cubeindex][i]]);

        pObject->vertices.push_back(
            { { vertlist[_triTable[cubeindex][i]].x, vertlist[_triTable[cubeindex][i]].y,
                  vertlist[_triTable[cubeindex][i]].z },
                normal, { r, g, b } });
        pObject->vertices.push_back(
            { { vertlist[_triTable[cubeindex][i + 1]].x, vertlist[_triTable[cubeindex][i + 1]].y,
                  vertlist[_triTable[cubeindex][i + 1]].z },
                normal, { r, g, b } });
        pObject->vertices.push_back(
            { { vertlist[_triTable[cubeindex][i + 2]].x, vertlist[_triTable[cubeindex][i + 2]].y,
                  vertlist[_triTable[cubeindex][i + 2]].z },
                normal, { r, g, b } });

        ntriang++;
    }

    if (ntriang > 0) {
        int size = pObject->vertices.size() - 1;

        for (int i = 0; i < ntriang; i++) {
            int step = i * 3;
            pObject->indices.push_back(size - step - 2);
            pObject->indices.push_back(size - step - 1);
            pObject->indices.push_back(size - step);
        }
    }
}

glm::vec3 Tools::VertexInterp(
    double isolevel, glm::vec3 p1, glm::vec3 p2, double valp1, double valp2)
{
    double mu;
    glm::vec3 p;

    if (std::abs(isolevel - valp1) < 0.00001)
        return p1;
    if (std::abs(isolevel - valp2) < 0.00001)
        return p2;
    if (std::abs(valp1 - valp2) < 0.00001)
        return p1;

    mu = (isolevel - valp1) / (valp2 - valp1);
    p.x = (float)(p1.x + mu * (p2.x - p1.x));
    p.y = (float)(p1.y + mu * (p2.y - p1.y));
    p.z = (float)(p1.z + mu * (p2.z - p1.z));

    return p;
}
