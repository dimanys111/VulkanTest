#pragma once

#include "stdinclude.h"

class Device;
class Graphics;
class GameObject;
class Cube;
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

private:
    std::shared_ptr<Graphics> m_graphics;

    std::shared_ptr<Cube> m_cube;

    std::shared_ptr<SkyBox> m_skyBox;

    std::shared_ptr<Camera> m_camera;

    std::shared_ptr<DirLight> m_dirLight;

    std::shared_ptr<Device> m_device;

    std::vector<std::vector<std::vector<GameObject*>>> m_chunks;
};
