#pragma once

#include "stdinclude.h"

#include "skyBox.h"
#include "gameObject.h"

#include "dirLight.h"

#include "device.h"
#include "graphics.h"

class Game{
public:
    Game(Device* device, Graphics* graphics);
    ~Game();
    void Init();

    void Update(float time);

    void Draw(VkCommandBuffer cmd, int indx);

    Graphics* graphics;

    SkyBox* skyBox;
    
    Camera* camera;

    GameObject* gameObject;

    DirLight* dirLight;

    Device* device; 

    std::vector<std::vector<std::vector<GameObject*>>> chunks;

};
