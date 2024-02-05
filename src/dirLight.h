#pragma once

#include "gameObject.h"
#include "stdinclude.h"
class Camera;
class Device;

class DirLight : public GameObject {
public:
    DirLight(
        std::shared_ptr<Device> device, std::shared_ptr<Camera> camera, const ShadersPath& paths);
    ~DirLight();
    void Update(float deltaTime);
    void SetTarget(glm::vec3 pos);

private:
    float m_distance { 250 };
    float m_angle { 0.f };

    glm::vec3 m_sunPos;
    glm::vec3 m_target;
};
