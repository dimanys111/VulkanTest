#include "game.h"
#include "camera.h"
#include "dirLight.h"
#include "gameObject.h"
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

    gameObject = std::make_shared<GameObject>(device, camera);
    gameObject->SetShadersName("shaders/objVert.spv", "shaders/objFrag.spv");
    gameObject->SetSize(glm::vec3(2.0f, 2.0f, 2.0f));
    gameObject->SetPosition({ 0, 0, 6 });
    gameObject->LoadTexture("textures/text2.png");
    gameObject->LoadModel("models/model.obj");
    gameObject->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
    gameObject->applyLight = true;
    gameObject->Init();

    skyBox = std::make_shared<SkyBox>(
        device, camera, ShadersPath { "shaders/skyVert.spv", "shaders/skyFrag.spv" });

    dirLight = std::make_shared<DirLight>(
        device, camera, ShadersPath { "shaders/sunVert.spv", "shaders/sunFrag.spv" });

    graphics->SetGameObject(gameObject);
    graphics->SetGameObject(skyBox->go);
    graphics->SetGameObject(dirLight->go);
}

void Game::Update(float time)
{

    camera->Update(time);
    gameObject->Rotating(glm::vec3(0, 0, 1) * time);
    gameObject->Update(time);

    dirLight->SetTarget(camera->GetPosition());
    dirLight->Update(time);

    skyBox->Update(time);
}

void Game::Draw(VkCommandBuffer cmd, int indx)
{
    gameObject->Draw(cmd, indx);

    skyBox->go->Draw(cmd, indx);

    dirLight->go->Draw(cmd, indx);
}
