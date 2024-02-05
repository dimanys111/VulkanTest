#pragma once

#include "gameObject.h"
#include "stdinclude.h"
class Camera;
class Device;

class Cube : public GameObject {
public:
    Cube(std::shared_ptr<Device> device, std::shared_ptr<Camera> camera, const ShadersPath& paths);
    ~Cube();
    void Update(float deltaTime);
};
