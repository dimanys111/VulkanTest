#include "gameObject.h"
#include "camera.h"
#include "device.h"
#include "model.h"
#include "pipeline.h"
#include "resource.h"

GameObject::GameObject(std::shared_ptr<Device> device, std::shared_ptr<Camera> camera)
    : Entity(device, camera)
    , m_applyLight(false)
    , m_size(1.0f)
    , m_model(glm::mat4(1.0f))
{
}

GameObject::~GameObject() { }

void GameObject::Init() { Entity::Init(); }

void GameObject::Update(float deltaTime)
{
    AppyTransforms(deltaTime);
    updateUniformBuffer(deltaTime);
}

void GameObject::setVertex(const std::vector<Vertex>& vertices)
{
    this->modelObj->vertices = vertices;
}

void GameObject::setIndices(const std::vector<uint16_t>& indices)
{
    this->modelObj->indices = indices;
}

void GameObject::SetPosition(glm::vec3 position)
{
    position /= 10;
    this->m_position = position;
}

void GameObject::AppyTransforms(float deltaTime)
{

    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_size);
    glm::mat4 rotationMatrix;
    rotationMatrix = glm::rotate(rotationMatrix, m_rotate.x, glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, m_rotate.y, glm::vec3(0, 1, 0));
    rotationMatrix = glm::rotate(rotationMatrix, m_rotate.z, glm::vec3(0, 0, 1));
    glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), m_position);

    m_model = translateMatrix * rotationMatrix * scaleMatrix * deltaTime;
}

glm::vec3 GameObject::GetPosition() { return m_position * 10.0f; }

void GameObject::SetSize(glm::vec3 size) { this->m_size = size; }

void GameObject::SetShadersName(const std::string_view& vertFile, const std::string_view& fragFile)
{
    this->m_vertFile = vertFile;
    this->m_fragFile = fragFile;
}

void GameObject::SetRotation(glm::vec3 rotation)
{
    rotation /= 10.0f;
    this->m_rotate = rotation;
}

void GameObject::Rotating(glm::vec3 rotation)
{
    rotation /= 10.0f;
    this->m_rotate += rotation;
}

glm::vec3 GameObject::GetRotate() const { return m_rotate * 10.0f; }

void GameObject::updateUniformBuffer(float deltaTime) const
{
    UniformBufferObject ubo {};
    ubo.model = m_model;
    ubo.view = camera->view();
    ubo.proj = camera->proj();

    ubo.sunDir = m_applyLight ? Resource::sunDir : glm::vec3(1.0f, -3.0f, -1.0f);

    void* data;
    vkMapMemory(device->device(), pipeline->uniformBuffersMemory[Resource::currentImage], 0,
        sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device->device(), pipeline->uniformBuffersMemory[Resource::currentImage]);
}
