#pragma once

#include "entity.h"
#include "stdinclude.h"

class Device;
class Camera;

class GameObject : public Entity {
public:
    GameObject(std::shared_ptr<Device> device, std::shared_ptr<Camera> camera);
    ~GameObject();
    void Init() override;

    void Update(float deltaTime);

    void setVertex(const std::vector<Vertex>& vertices);

    void setIndices(const std::vector<uint16_t>& indices);

    void SetPosition(glm::vec3 position);

    void AppyTransforms(float deltaTime);

    glm::vec3 GetPosition();

    void SetSize(glm::vec3 size);

    void SetShadersName(const std::string& vertFile, const std::string& fragFile);

    void SetRotation(glm::vec3 rotation);

    void Rotating(glm::vec3 rotation);

    glm::vec3 GetRotate();

    std::string vertFile, fragFile;

    bool applyLight;

private:
    void updateUniformBuffer(float deltaTime);

    glm::vec3 size;

    glm::mat4 model;
    glm::vec3 position;
    glm::vec3 rotate;
};
