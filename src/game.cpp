#include "game.h"
#include "camera.h"
#include "dirLight.h"
#include "gameObject.h"
#include "cube.h"
#include "graphics.h"
#include "resource.h"
#include "skyBox.h"

Game::Game(std::shared_ptr<Device> device, std::shared_ptr<Graphics> graphics)
{
    this->device = device;
    this->graphics = graphics;
}

Game::~Game() { }

void Game::Init()
{

    camera = std::make_shared<Camera>(
        Resource::swapChainExtent.width, Resource::swapChainExtent.height);

    cube = std::make_shared<Cube>(
        device, camera, ShadersPath { "shaders/objVert.spv", "shaders/objFrag.spv" });

    skyBox = std::make_shared<SkyBox>(
        device, camera, ShadersPath { "shaders/skyVert.spv", "shaders/skyFrag.spv" });

    dirLight = std::make_shared<DirLight>(
        device, camera, ShadersPath { "shaders/sunVert.spv", "shaders/sunFrag.spv" });

    graphics->SetGameObject(cube->go);
    graphics->SetGameObject(skyBox->go);
    graphics->SetGameObject(dirLight->go);
}

void Game::Update(float time)
{

    camera->Update(time);

    cube->Update(time);

    dirLight->SetTarget(camera->GetPosition());
    dirLight->Update(time);

    skyBox->Update(time);
}

void Game::Draw(VkCommandBuffer cmd, int indx)
{
    cube->go->Draw(cmd, indx);

    skyBox->go->Draw(cmd, indx);

    dirLight->go->Draw(cmd, indx);
}
