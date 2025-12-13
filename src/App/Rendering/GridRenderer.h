#pragma once
#include "RenderingSystem.h"
#include "App/Engine/Camera.h"
#include "Core/Event.h"


class GridRenderer {
public:
    void OnEvent(Core::Event &event);
    void RenderGrid(RenderingSystem & renderer) const;

    glm::vec4 gridColor_ = glm::vec4(0.2f, 0.2f, 0.2f, .4f);
    float gridSpacing_ = 2.0f;
    bool showGrid = true;

private:
    Camera *camera_ = nullptr;
};
