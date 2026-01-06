#include "ArrowRenderer.h"

#include "Core/AppLayer.h"

ArrowRenderer::ArrowRenderer(const LineRenderer &renderer, const RenderingSystem *renderingSystem)
        : BaseRenderer(renderingSystem), lineRenderer_(renderer) {}

void ArrowRenderer::RenderArrow(const glm::vec2 origin, const glm::vec2 direction, const glm::vec4 color, const int width) const {
        const glm::vec2 tip = origin + direction;

        constexpr float arrowHeadLength = .1f;
        const glm::vec2 dirNorm = glm::normalize(direction);
        const glm::vec2 leftHead = tip - arrowHeadLength * dirNorm - arrowHeadLength * glm::vec2(-dirNorm.y, dirNorm.x);
        const glm::vec2 rightHead = leftHead + 2.0f * arrowHeadLength * glm::vec2(-dirNorm.y, dirNorm.x);

        lineRenderer_.RenderLine(origin, tip, width, color);
        lineRenderer_.RenderLine(tip, leftHead, width, color);
        lineRenderer_.RenderLine(tip, rightHead, width, color);
}
