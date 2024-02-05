#include "graphics.h"
#include "device.h"
#include "gameObject.h"
#include "pipeline.h"
#include "renderer.h"
#include "resource.h"
#include "swapchain.h"
#include "tools.h"

Graphics::Graphics(std::shared_ptr<WindowManager> window, std::shared_ptr<Device> device,
    std::shared_ptr<SwapChain> swapchain)
{
    m_window = window;
    m_device = device;
    m_swapchain = swapchain;

    m_renderer = std::make_shared<Renderer>(m_device);
}

Graphics::~Graphics()
{

    vkDestroyImageView(m_device->device(), m_depthImageView, nullptr);

    vkDestroyImage(m_device->device(), m_depthImage, nullptr);
    vkFreeMemory(m_device->device(), m_depthImageMemory, nullptr);

    for (auto framebuffer : m_swapChainFramebuffers) {
        vkDestroyFramebuffer(m_device->device(), framebuffer, nullptr);
    }
}

void Graphics::SetGameObject(std::shared_ptr<GameObject> go) { m_gameObjects.push_back(go); }

void Graphics::Init()
{
    createDepthResources();
    createFramebuffers();
    for (const auto& go : m_gameObjects) {
        go->pipeline()->createGraphicsPipeline(
            go->vertFile(), go->fragFile(), m_renderer->renderPass());
    }
    createCommandBuffers();
}

void Graphics::createFramebuffers()
{
    m_swapChainFramebuffers.resize(Resource::countFrames);
    for (size_t i = 0; i < m_swapchain->swapChainImageViews().size(); i++) {
        std::array<VkImageView, 2> attachments
            = { m_swapchain->swapChainImageViews()[i], m_depthImageView };

        VkFramebufferCreateInfo framebufferInfo {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderer->renderPass();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = Resource::swapChainExtent.width;
        framebufferInfo.height = Resource::swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(
                m_device->device(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i])
            != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void Graphics::createDepthResources()
{
    VkFormat depthFormat = m_renderer->findDepthFormat();

    Tools::createImage(Resource::swapChainExtent.width, Resource::swapChainExtent.height,
        depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage, m_depthImageMemory);
    m_depthImageView = Tools::createImageView(m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void Graphics::createCommandBuffers()
{
    m_commandBuffers.resize(Resource::countFrames);

    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = Resource::commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

    if (vkAllocateCommandBuffers(m_device->device(), &allocInfo, m_commandBuffers.data())
        != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Graphics::setCommandBuffers()
{
    for (size_t i = 0; i < m_commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_renderer->renderPass();
        renderPassInfo.framebuffer = m_swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = Resource::swapChainExtent;

        std::array<VkClearValue, 2> clearValues {};
        clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        for (const auto& go : m_gameObjects) {
            vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                go->pipeline()->graphicsPipeline());

            go->Draw(m_commandBuffers[i], i);
        }

        vkCmdEndRenderPass(m_commandBuffers[i]);

        if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}
