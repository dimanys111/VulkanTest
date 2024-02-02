#pragma once

#include "stdinclude.h"

class Device;
class Camera;
class GameObject;

class SkyBox {
public:
    SkyBox(Device* device, Camera* camera, ShadersPath paths);
    ~SkyBox();

    void Update(float deltaTime);

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    glm::vec3 colorTop;
    glm::vec3 colorBottom;

    GameObject* go;
    Camera* camera;
};
