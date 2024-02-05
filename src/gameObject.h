#pragma once

#include "entity.h"
#include "stdinclude.h"

class Device;
class Camera;

class GameObject : public Entity {
public:
    GameObject(std::shared_ptr<Device> device, std::shared_ptr<Camera> camera);
    virtual ~GameObject();
    void Init() override;

    void Update(float deltaTime);

    std::string& fragFile() { return m_fragFile; }

    std::string& vertFile() { return m_vertFile; }

protected:
    void setVertex(const std::vector<Vertex>& vertices);

    void setIndices(const std::vector<uint16_t>& indices);

    void SetPosition(glm::vec3 position);

    void AppyTransforms(float deltaTime);

    glm::vec3 GetPosition();

    void SetSize(glm::vec3 size);

    void SetShadersName(const std::string_view& vertFile, const std::string_view& fragFile);

    void SetRotation(glm::vec3 rotation);

    void Rotating(glm::vec3 rotation);

    glm::vec3 GetRotate() const;

    void updateUniformBuffer(float deltaTime) const;

    glm::vec3 m_size;

    glm::mat4 m_model;
    glm::vec3 m_position;
    glm::vec3 m_rotate;

    std::string m_vertFile;
    std::string m_fragFile;

    bool m_applyLight;
};
