#pragma once

#include "common/utils.h"
#include "Lights.h"
#include <iostream>
#include "Object3D.h"


class Renderer {
private:
    std::vector<Object3D*> m_Objects;
    std::vector<PointLight*> m_PointLights;
public:
    void Render(Camera* cam);
    void AddObject3D(Object3D *obj);
    void AddPointLight(PointLight *light);
};