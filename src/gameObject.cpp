#include "gameObject.h"


GameObject::GameObject(Device *device, Camera *camera) : Entity(device, camera), size(1.0f)
{
    model = glm::mat4(1.0f);
}

GameObject::~GameObject(){

}

void GameObject::Init(){
    Entity::Init();

}

void GameObject::Update(float deltaTime)
{
    AppyTransforms(deltaTime);
    updateUniformBuffer(deltaTime);
}

void GameObject::setVertex(std::vector<Vertex> vertices)
{
    this->m_model->vertices = vertices;
}

void GameObject::setIndices(std::vector<uint16_t> indices)
{
    this->m_model->indices = indices;
}

void GameObject::SetPosition(glm::vec3 position)
{
    position /= 10;
    this->position = position;
}

void GameObject::AppyTransforms(float deltaTime)
{

    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), size);
    glm::mat4 rotationMatrix;
    rotationMatrix = glm::rotate(rotationMatrix, rotate.x, glm::vec3(1,0,0));
    rotationMatrix = glm::rotate(rotationMatrix, rotate.y, glm::vec3(0,1,0));
    rotationMatrix = glm::rotate(rotationMatrix, rotate.z, glm::vec3(0,0,1));
    glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), position);

    model = translateMatrix * rotationMatrix * scaleMatrix * deltaTime;

}

glm::vec3 GameObject::GetPosition()
{
    return position * 10.0f;
}

void GameObject::SetSize(glm::vec3 size)
{
    this->size = size;
}

void GameObject::SetShadersName(std::string vertFile, std::string fragFile)
{
    this->vertFile = vertFile;
    this->fragFile = fragFile;
}

void GameObject::SetRotation(glm::vec3 rotation)
{
    rotation /=10.0f;
    this->rotate = rotation;

}

void GameObject::Rotating(glm::vec3 rotation)
{
    rotation /=10.0f;
    this->rotate += rotation;

}

glm::vec3 GameObject::GetRotate()
{
    return rotate * 10.0f;
}

void GameObject::updateUniformBuffer(float deltaTime) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = model;
    ubo.view = camera->view;
    ubo.proj = camera->proj;

    ubo.sunDir = applyLight ? Resource::sunDir : glm::vec3(1.0f, -3.0f, -1.0f);

    void* data;
    vkMapMemory(device->device, pipeline->uniformBuffersMemory[Resource::currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device->device, pipeline->uniformBuffersMemory[Resource::currentImage]);

}
