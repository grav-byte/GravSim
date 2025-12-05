//
// Created by Lorenz Saalmann on 05.12.25.
//

#pragma once
#include "IRenderer.h"


class CircleRenderer: public IRenderer {
public:
    ~CircleRenderer() override;
protected:
    void Render(RenderingSystem& rendering, Transform transform) override;
};
