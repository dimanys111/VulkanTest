#pragma once

#include "stdinclude.h"

class Device;
class Camera;
class GameObject;

class SkyBox {
public:
    SkyBox(Device* device, Camera* camera, const ShadersPath& paths);
    ~SkyBox();

    void Update(float deltaTime);

    glm::vec3 colorTop;
    glm::vec3 colorBottom;

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    GameObject* go;
    Camera* m_camera;
};
