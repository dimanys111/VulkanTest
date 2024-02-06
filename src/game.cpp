#include "game.h"
#include "camera.h"
#include "cube.h"
#include "dirLight.h"
#include "gameObject.h"
#include "graphics.h"
#include "resource.h"
#include "skyBox.h"

Game::Game(std::shared_ptr<Device> device, std::shared_ptr<Graphics> graphics)
    : m_device(device)
    , m_graphics(graphics)
{
}

Game::~Game() { }

void Game::Init()
{

    m_camera = std::make_shared<Camera>(
        Resource::swapChainExtent.width, Resource::swapChainExtent.height);

    m_cube = std::make_shared<Cube>(
        m_device, m_camera, ShadersPath { "shaders/objVert.spv", "shaders/objFrag.spv" });

    m_skyBox = std::make_shared<SkyBox>(
        m_device, m_camera, ShadersPath { "shaders/skyVert.spv", "shaders/skyFrag.spv" });

    m_dirLight = std::make_shared<DirLight>(
        m_device, m_camera, ShadersPath { "shaders/sunVert.spv", "shaders/sunFrag.spv" });

    m_graphics->SetGameObject(m_cube);
    m_graphics->SetGameObject(m_skyBox);
    m_graphics->SetGameObject(m_dirLight);
}

void Game::Update(float time)
{

    m_camera->Update(time);

    m_cube->Update(time);

    m_dirLight->SetTarget(m_camera->GetPosition());
    m_dirLight->Update(time);

    m_skyBox->Update(time);
}

void Game::Draw(VkCommandBuffer cmd, int indx)
{
    m_cube->Draw(cmd, indx);

    m_skyBox->Draw(cmd, indx);

    m_dirLight->Draw(cmd, indx);
}
