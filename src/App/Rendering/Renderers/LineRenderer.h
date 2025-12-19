#pragma once
#include "SpriteRenderer.h"


class LineRenderer : public SpriteRenderer {
public:
    explicit LineRenderer(const RenderingSystem* renderer);

    void RenderLine(const glm::vec2& start, const glm::vec2& end, int lineThickPx, const glm::vec4 &color) const;
};
