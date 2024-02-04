#include "cube.h"
#include "gameObject.h"
#include "resource.h"
#include "tools.h"

Cube::Cube(
    std::shared_ptr<Device> device, std::shared_ptr<Camera> camera, const ShadersPath& paths)
{
    this->camera = camera;

    go = std::make_shared<GameObject>(device, camera);
    go->SetShadersName(paths.vertShader, paths.fragShader);
    go->SetSize(glm::vec3(2.0f, 2.0f, 2.0f));
    go->SetPosition({ 0, 0, 6 });
    go->LoadTexture("textures/text2.png");
    go->LoadModel("models/model.obj");
    go->SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
    go->applyLight = true;
    go->Init();
}

Cube::~Cube() { }

void Cube::Update(float deltaTime)
{
    go->Rotating(glm::vec3(0, 0, 1) * deltaTime);
    go->Update(deltaTime);
}
