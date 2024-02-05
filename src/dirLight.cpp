#include "dirLight.h"
#include "gameObject.h"
#include "resource.h"
#include "tools.h"

DirLight::DirLight(
    std::shared_ptr<Device> device, std::shared_ptr<Camera> camera, const ShadersPath& paths)
    : GameObject(device, camera)
    , m_angle(0.f)
{
    PrimitiveObject pObject = Tools::GetPrimitives(PrimitiveType::PRIMITIVE_TYPE_QUAD);
    pObject.vertices[0].color = Vertex::ColorGet(76, 60, 24);
    pObject.vertices[1].color = Vertex::ColorGet(76, 60, 24);
    pObject.vertices[2].color = Vertex::ColorGet(76, 60, 24);
    pObject.vertices[3].color = Vertex::ColorGet(76, 60, 24);

    SetShadersName(paths.vertShader, paths.fragShader);
    setVertex(pObject.vertices);
    setIndices(pObject.indices);
    m_applyLight = false;
    Init();

    m_distance = 250;
    Resource::sunDir = glm::vec3(1, -1, 1);
}

DirLight::~DirLight() { }

void DirLight::Update(float deltaTime)
{
    int del = 1000;
    float resultAngle = m_angle / del;
    m_sunPos = m_target;
    m_sunPos.x -= m_distance * cos(resultAngle);
    m_sunPos.z -= m_distance * sin(resultAngle);

    m_angle++;

    if (m_angle >= 360 * del)
        m_angle = 0;

    SetPosition(m_sunPos);
    SetRotation(glm::vec3(0, -resultAngle + 90, 0) * 10.0f);
    Resource::sunDir = glm::vec3(cos(resultAngle), -0.5f, sin(resultAngle));
    GameObject::Update(deltaTime);
}

void DirLight::SetTarget(glm::vec3 pos) { m_target = pos; }
