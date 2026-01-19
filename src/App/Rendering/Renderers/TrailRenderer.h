#pragma once
#include "IRenderer.h"
#include "LineRenderer.h"
#include "App/Engine/SceneObject.h"


class TrailRenderer : public IRenderer {
public:
    explicit TrailRenderer(const LineRenderer &renderer, const RenderingSystem *renderingSystem);

    void RenderTrail(const SceneObject &obj, int width = 4);
    void Clear();
    inline static uint16_t MaxTrailLength = 100;
private:
    std::unordered_map<uint32_t, std::vector<glm::vec2>> trails_; // map of object ID to trail points
    LineRenderer lineRenderer_;
};
