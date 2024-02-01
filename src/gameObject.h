#pragma once

#include "stdinclude.h"

#include "camera.h"
#include "device.h"
#include "entity.h"
#include "resource.h"

class GameObject : public Entity {
public:
    GameObject(Device* device, Camera* camera);
    ~GameObject();
    void Init();

    void Update(float deltaTime);

    void setVertex(std::vector<Vertex> vertices);

    void setIndices(std::vector<uint16_t> indices);

    void SetPosition(glm::vec3 position);

    void AppyTransforms(float deltaTime);

    glm::vec3 GetPosition();

    void SetSize(glm::vec3 size);

    void SetShadersName(std::string vertFile, std::string fragFile);

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
