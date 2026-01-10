#include "TrailRenderer.h"

TrailRenderer::TrailRenderer(const LineRenderer &renderer, const RenderingSystem *renderingSystem)
    : BaseRenderer(renderingSystem), lineRenderer_(renderer) {
    MaxTrailLength = 500;
}

void TrailRenderer::RenderTrail(const SceneObject &obj, const int width) {
    auto& trail = trails_[obj.id];    // this is default constructed if the key is not yet present

    // add current position to trail
    trail.push_back(obj.transform.position);

    // limit trail length
    if (trail.size() > MaxTrailLength) {
        trail.erase(trail.begin(), trail.begin() + (trail.size() - MaxTrailLength));
    }

    // render the trail as many lines
    auto color = obj.visual ? obj.visual->color : glm::vec4(1.0f);
    glm::vec2 nextStartPoint = trail[0];
    for (int i = 1; i < trail.size(); ++i) {
        // fade color based on segment index
        auto currentPoint = trail[i];

        // only render if the points are sufficiently far apart
        bool renderSegment = dot(nextStartPoint, currentPoint) > .1;
        if (renderSegment) {
            auto col = color;
            col.a *= static_cast<float>(i) / trail.size();
            if (col.a > 0.01f)
                lineRenderer_.RenderLine(nextStartPoint, currentPoint, width, col);
            nextStartPoint = currentPoint;
        }
    }
}

void TrailRenderer::Clear() {
    trails_.clear();
}


