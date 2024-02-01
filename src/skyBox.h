#pragma once

#include "stdinclude.h"

#include "device.h"

#include "camera.h"
#include "gameObject.h"

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
