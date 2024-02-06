#pragma once

#include "gameObject.h"
#include "stdinclude.h"
class Device;
class Camera;

class SkyBox : public GameObject {
public:
    SkyBox(
        std::shared_ptr<Device> device, std::shared_ptr<Camera> camera, const ShadersPath& paths);
    ~SkyBox();

    void Update(float deltaTime) override;

private:
    glm::vec3 m_colorTop;
    glm::vec3 m_colorBottom;

    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
};
