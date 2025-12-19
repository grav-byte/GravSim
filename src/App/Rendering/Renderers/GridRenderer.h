#pragma once
#include "LineRenderer.h"

class GridRenderer : public BaseRenderer {
public:
    explicit GridRenderer(const LineRenderer& renderer, const RenderingSystem* renderingSystem);

    void RenderGrid(const glm::vec4 &grid_color, float grid_spacing) const;
private:
    LineRenderer lineRenderer_;
};
