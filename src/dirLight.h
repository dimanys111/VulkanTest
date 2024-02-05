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

    std::shared_ptr<Camera> camera;

    float distance;
    float angle;

    glm::vec3 sunPos;
    glm::vec3 target;
};
