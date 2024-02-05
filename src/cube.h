#pragma once

#include "stdinclude.h"

class Camera;
class Device;
class GameObject;

class Cube {
public:
    Cube(std::shared_ptr<Device> device, std::shared_ptr<Camera> camera, const ShadersPath& paths);
    ~Cube();
    void Update(float deltaTime);

    std::shared_ptr<GameObject> go;

    std::shared_ptr<Camera> camera;
};
