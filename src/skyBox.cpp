#include "skyBox.h"
#include "camera.h"
#include "gameObject.h"

SkyBox::SkyBox(
    std::shared_ptr<Device> device, std::shared_ptr<Camera> camera, const ShadersPath& paths)
    : GameObject(device, camera)
    , colorBottom({ 0.0f, 0.0f, 0.1f })
    , colorTop({ 0.1f, 0.1f, 1.0f })
    , vertices({
          // Back
          { { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          { { 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          { { -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          // Front
          { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          { { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          { { 1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          { { -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          // Left
          { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          { { -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          { { -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          { { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          // Right
          { { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          { { 1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          { { 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          // Top
          { { -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          { { -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          { { 1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorTop },
          // Bottom
          { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          { { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          { { 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
          { { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, colorBottom },
      })
    , indices({ 0, 1, 2, 2, 3, 0, 6, 5, 4, 4, 7, 6, 10, 9, 8, 8, 11, 10, 12, 13, 14, 14, 15, 12, 16,
          17, 18, 18, 19, 16, 22, 21, 20, 20, 23, 22 })
{
    SetShadersName(paths.vertShader, paths.fragShader);
    float size = Camera::ViewDistance / 2;
    SetSize(glm::vec3(size, size, size));
    setVertex(vertices);
    setIndices(indices);
    m_applyLight = false;
    Init();
}

SkyBox::~SkyBox() { }

void SkyBox::Update(float deltaTime)
{
    glm::vec3 pos = m_camera->GetPosition();
    SetPosition(pos);
    GameObject::Update(deltaTime);
}
