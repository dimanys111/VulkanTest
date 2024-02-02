#include "game.h"
#include "camera.h"
#include "dirLight.h"
#include "gameObject.h"
#include "graphics.h"
#include "resource.h"
#include "skyBox.h"

Game::Game(Device* device, Graphics* graphics)
{
    this->device = device;
    this->graphics = graphics;

    Init();
}

Game::~Game()
{

    delete gameObject;
    delete skyBox;
    delete dirLight;
    delete camera;
}

void Game::Init()
{

    camera = new Camera(Resource::swapChainExtent.width, Resource::swapChainExtent.height);

    gameObject = new GameObject(device, camera);
    gameObject->SetShadersName("shaders/vert.spv", "shaders/frag.spv");
    gameObject->SetSize(glm::vec3(2.0f, 2.0f, 2.0f));
    gameObject->SetPosition({ 0, 0, 6 });
    gameObject->LoadTexture("textures/text2.png");
    gameObject->LoadModel("models/model.obj");
    gameObject->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
    gameObject->applyLight = true;
    gameObject->Init();

    skyBox = new SkyBox(device, camera, { "shaders/skyVert.spv", "shaders/skyFrag.spv" });

    dirLight = new DirLight(device, camera, { "shaders/sunV.spv", "shaders/sunF.spv" });

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
