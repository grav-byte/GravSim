#pragma once
#include "IRenderer.h"
#include "LineRenderer.h"


class ArrowRenderer : public IRenderer {
public:
    explicit ArrowRenderer(const LineRenderer& renderer, const RenderingSystem* renderingSystem);

    void RenderArrow(const SceneObject::DebugArrow &arrow) const;
private:
    LineRenderer lineRenderer_;
};
