#pragma once

#include "stdinclude.h"

class Device;
class Graphics;
class GameObject;
class DirLight;
class SkyBox;
class Camera;

class Game {
public:
    Game(std::shared_ptr<Device> device, std::shared_ptr<Graphics> graphics);
    ~Game();
    void Init();

    void Update(float time);

    void Draw(VkCommandBuffer cmd, int indx);

    std::shared_ptr<Graphics> graphics;

    std::shared_ptr<SkyBox> skyBox;

    std::shared_ptr<Camera> camera;

    std::shared_ptr<GameObject> gameObject;

    std::shared_ptr<DirLight> dirLight;

    std::shared_ptr<Device> device;

    std::vector<std::vector<std::vector<GameObject*>>> chunks;
};
