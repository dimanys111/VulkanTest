#include "engine.h"
#include "device.h"
#include "game.h"
#include "graphics.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "renderer.h"
#include "resource.h"
#include "swapchain.h"
#include "tools.h"
#include "window.h"

Engine::Engine() = default;

Engine::~Engine()
{

    for (size_t i = 0; i < Resource::countFrames; i++) {
        vkDestroySemaphore(m_device->device(), m_renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(m_device->device(), m_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_device->device(), m_inFlightFences[i], nullptr);
    }

    vkDestroyDescriptorPool(m_device->device(), m_imguiPool, nullptr);

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(nullptr);
    }
}

void Engine::Init()
{
    printf("Initializing vulkan instance.");

    m_window = std::make_shared<WindowManager>();
    m_window->Init();

    createInstance();
    setupDebugMessenger();
    m_window->createSurface();

    m_device = std::make_shared<Device>(m_window);
    m_device->Init();

    Tools::setDevice(m_device);

    m_swapchain = std::make_shared<SwapChain>(m_window, m_device);
    m_swapchain->Init();

    m_graphics = std::make_shared<Graphics>(m_window, m_device, m_swapchain);

    m_game = std::make_shared<Game>(m_device, m_graphics);
    m_game->Init();

    GUIInit();

    m_graphics->Init();

    createSyncObjects();

    m_v[0] = Resource::sunDir.x;
    m_v[1] = Resource::sunDir.y;
    m_v[2] = Resource::sunDir.z;
}

void Engine::createInstance()
{
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(Device::validationLayers.size());
        createInfo.ppEnabledLayerNames = Device::validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_window->instance()) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void Engine::setupDebugMessenger()
{
    if (!enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo {};
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(&createInfo, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

bool Engine::checkValidationLayerSupport() const
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : Device::validationLayers) {
        bool layerFound = false;

        if (std::any_of(
                availableLayers.cbegin(), availableLayers.cend(), [&](const auto& layerProperties) {
                    return strcmp(layerName, layerProperties.layerName) == 0;
                })) {
            layerFound = true;
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

void Engine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

VkResult Engine::CreateDebugUtilsMessengerEXT(
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        m_window->instance(), "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(m_window->instance(), pCreateInfo, pAllocator, &m_debugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Engine::DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        m_window->instance(), "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(m_window->instance(), m_debugMessenger, pAllocator);
    }
}

VkBool32 Engine::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

std::vector<const char*> Engine::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void Engine::Run()
{
    m_graphics->setCommandBuffers();
    while (!m_window->GetClose()) {
        glfwPollEvents();

        draw();

        if (Resource::pressed[GLFW_KEY_ESCAPE])
            glfwSetWindowShouldClose(m_window->window(), GL_TRUE);
    }

    vkDeviceWaitIdle(m_device->device());

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Engine::GUIInit()
{

    // Create Descriptor Pool

    VkDescriptorPoolSize pool_sizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 }, { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 }, { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 } };
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000;
    pool_info.poolSizeCount = std::size(pool_sizes);
    pool_info.pPoolSizes = pool_sizes;

    vkCreateDescriptorPool(m_device->device(), &pool_info, nullptr, &m_imguiPool);

    ImGui::CreateContext();
    const ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
    // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(m_window->window(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = m_window->instance();
    init_info.PhysicalDevice = m_device->physicalDevice();
    init_info.Device = m_device->device();
    init_info.QueueFamily = m_device->queueFamily();
    init_info.Queue = m_device->graphicsQueue();
    init_info.DescriptorPool = m_imguiPool;
    init_info.MinImageCount = 2;
    init_info.ImageCount = 3;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&init_info, m_graphics->renderer()->renderPass());

    {
        // Use any command queue
        VkCommandBuffer command_buffer = Tools::beginSingleTimeCommands();
        Tools::endSingleTimeCommands(command_buffer);
    }
}

void Engine::Update()
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime)
                     .count()
        / 1000;

    m_game->Update(time);

    m_v[0] = Resource::sunDir.x;
    m_v[1] = Resource::sunDir.y;
    m_v[2] = Resource::sunDir.z;

    if (Resource::pressed[GLFW_KEY_LEFT_CONTROL] && !m_menuSwaped) {
        Resource::showCursor = !Resource::showCursor;
        m_menuSwaped = true;
    }

    if (!Resource::pressed[GLFW_KEY_LEFT_CONTROL])
        m_menuSwaped = false;

    // if (!Resource::showCursor)
    //     glfwSetInputMode(window->window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // else
    //     glfwSetInputMode(window->window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Engine::draw()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and
                                       // append into it.

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::InputFloat3("Sun Dir", m_v);
        ImGui::End();
    }

    ImGui::Render();

    uint32_t imageIndex;

    FrameRender(&imageIndex);
    FramePresent(imageIndex);
}

void Engine::FrameRender(uint32_t* imageIndex)
{
    VkResult err;

    vkWaitForFences(m_device->device(), 1, &m_inFlightFences[m_currentFrame], VK_TRUE, UINT64_MAX);

    err = vkAcquireNextImageKHR(m_device->device(), m_swapchain->swapChain(), UINT64_MAX,
        m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, imageIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
        m_swapChainRebuild = true;
        return;
    }

    Resource::currentImage = m_currentFrame;

    Update();

    // Check if a previous frame is using this image (i.e. there is its fence to
    // wait on)
    if (m_imagesInFlight[*imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(m_device->device(), 1, &m_imagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
    }

    // Mark the image as now being in use by this frame
    m_imagesInFlight[*imageIndex] = m_inFlightFences[m_currentFrame];

    MakeFrame();

    VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = &wait_stage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_graphics->commandBuffers()[*imageIndex];

    VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(m_device->device(), 1, &m_inFlightFences[m_currentFrame]);

    vkQueueSubmit(m_device->graphicsQueue(), 1, &submitInfo, m_inFlightFences[m_currentFrame]);

    vkQueueWaitIdle(m_device->graphicsQueue());
}

void Engine::MakeFrame()
{

    vkResetCommandPool(m_device->device(), Resource::commandPool, 0);
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(m_graphics->commandBuffers()[m_currentFrame], &beginInfo);

    std::array<VkClearValue, 2> clearValues {};
    clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = m_graphics->renderer()->renderPass();
    info.framebuffer = m_graphics->swapChainFramebuffers()[m_currentFrame];
    info.renderArea.extent.width = Resource::swapChainExtent.width;
    info.renderArea.extent.height = Resource::swapChainExtent.height;
    info.clearValueCount = static_cast<uint32_t>(clearValues.size());
    info.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(
        m_graphics->commandBuffers()[m_currentFrame], &info, VK_SUBPASS_CONTENTS_INLINE);

    m_game->Draw(m_graphics->commandBuffers()[m_currentFrame], m_currentFrame);

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(
        ImGui::GetDrawData(), m_graphics->commandBuffers()[m_currentFrame]);

    // Submit command buffer
    vkCmdEndRenderPass(m_graphics->commandBuffers()[m_currentFrame]);

    vkEndCommandBuffer(m_graphics->commandBuffers()[m_currentFrame]);
}

void Engine::FramePresent(uint32_t imageIndex)
{
    if (m_swapChainRebuild)
        return;

    VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };

    VkSwapchainKHR swapChains[] = { m_swapchain->swapChain() };

    VkPresentInfoKHR presentInfo {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    VkResult err = vkQueuePresentKHR(m_device->presentQueue(), &presentInfo);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
        m_swapChainRebuild = true;
        return;
    }

    vkQueueWaitIdle(m_device->presentQueue());

    m_currentFrame = (m_currentFrame + 1) % Resource::countFrames;
}

void Engine::createSyncObjects()
{
    m_imageAvailableSemaphores.resize(Resource::countFrames);
    m_renderFinishedSemaphores.resize(Resource::countFrames);
    m_inFlightFences.resize(Resource::countFrames);
    m_imagesInFlight.resize(Resource::countFrames, VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < Resource::countFrames; i++) {
        if (vkCreateSemaphore(
                m_device->device(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i])
                != VK_SUCCESS
            || vkCreateSemaphore(
                   m_device->device(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i])
                != VK_SUCCESS
            || vkCreateFence(m_device->device(), &fenceInfo, nullptr, &m_inFlightFences[i])
                != VK_SUCCESS) {

            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}
