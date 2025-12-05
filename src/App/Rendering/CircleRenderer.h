//
// Created by Lorenz Saalmann on 05.12.25.
//

#pragma once
#include "IRenderer.h"


class CircleRenderer: public IRenderer {
public:
    CircleRenderer();
    CircleRenderer(glm::vec4 color);
    ~CircleRenderer() override;
    void Render(RenderingSystem& rendering, Transform transform) override;
};
