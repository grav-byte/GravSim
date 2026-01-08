#pragma once
#include "BaseRenderer.h"
#include "LineRenderer.h"


class ArrowRenderer : public BaseRenderer {
public:
    explicit ArrowRenderer(const LineRenderer& renderer, const RenderingSystem* renderingSystem);

    void RenderArrow(const SceneObject::DebugArrow &arrow, int width = 10) const;
private:
    LineRenderer lineRenderer_;
};
