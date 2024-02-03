#pragma once

#include "stdinclude.h"

class Device;
class Camera;
class GameObject;

class SkyBox {
public:
    SkyBox(std::shared_ptr<Device> device, std::shared_ptr<Camera> camera_, const ShadersPath& paths);
    ~SkyBox();

    void Update(float deltaTime);

    glm::vec3 colorTop;
    glm::vec3 colorBottom;

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    std::shared_ptr<GameObject> go;
    std::shared_ptr<Camera> camera;
};
