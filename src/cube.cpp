#include "cube.h"
#include "gameObject.h"
#include "resource.h"
#include "tools.h"

Cube::Cube(std::shared_ptr<Device> device, std::shared_ptr<Camera> camera, const ShadersPath& paths)
    : GameObject(device, camera)
{

    SetShadersName(paths.vertShader, paths.fragShader);
    SetSize(glm::vec3(2.0f, 2.0f, 2.0f));
    SetPosition({ 0, 0, 6 });
    LoadTexture("textures/text2.png");
    LoadModel("models/model.obj");
    SetFrontFace(VK_FRONT_FACE_COUNTER_CLOCKWISE);
    m_applyLight = true;
    Init();
}

Cube::~Cube() { }

void Cube::Update(float deltaTime)
{
    Rotating(glm::vec3(0, 0, 1) * deltaTime);
    GameObject::Update(deltaTime);
}
