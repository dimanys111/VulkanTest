#pragma once

#include "stdinclude.h"

#include "camera.h"

#include "tools.h"

#include "gameObject.h"
#include "resource.h"

class DirLight {
public:
    DirLight(Device* device, Camera* camera, ShadersPath paths);
    ~DirLight();
    void Update(float deltaTime);

    void SetTarget(glm::vec3 pos);

    GameObject* go;

    Camera* camera;

    float distance;
    float angle;

    glm::vec3 sunPos;
    glm::vec3 target;
};
