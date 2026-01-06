#pragma once
#include "BaseRenderer.h"
#include "LineRenderer.h"


class ArrowRenderer : public BaseRenderer {
public:
    explicit ArrowRenderer(const LineRenderer& renderer, const RenderingSystem* renderingSystem);

    void RenderArrow(glm::vec2 origin, glm::vec2 direction, glm::vec4 color, int width=10) const;
private:
    LineRenderer lineRenderer_;
};
