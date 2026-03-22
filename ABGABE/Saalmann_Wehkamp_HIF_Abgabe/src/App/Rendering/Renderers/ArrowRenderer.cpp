#include "ArrowRenderer.h"

#include "App/Engine/Scene.h"
#include "Core/AppLayer.h"

ArrowRenderer::ArrowRenderer(const LineRenderer &renderer, const RenderingSystem *renderingSystem)
        : IRenderer(renderingSystem), lineRenderer_(renderer) {}

void ArrowRenderer::RenderArrow(const SceneObject::DebugArrow &arrow) const {
        const glm::vec2 tip = arrow.origin + arrow.direction;

        const float arrowHeadLength = arrow.arrowHeadLength;
        const glm::vec2 dirNorm = glm::normalize(arrow.direction);
        const glm::vec2 leftHead = tip - arrowHeadLength * dirNorm - arrowHeadLength * glm::vec2(-dirNorm.y, dirNorm.x);
        const glm::vec2 rightHead = leftHead + 2.0f * arrowHeadLength * glm::vec2(-dirNorm.y, dirNorm.x);

        if (arrow.dashed) {
                for (float t = 0; t < 1.0f; t += 0.2f) {
                        const glm::vec2 start = arrow.origin + t * arrow.direction;
                        const glm::vec2 end = arrow.origin + glm::min(t + 0.1f, 1.0f) * arrow.direction;
                        lineRenderer_.RenderLine(start, end, arrow.width, arrow.color);
                }
        } else
                lineRenderer_.RenderLine(arrow.origin, tip, arrow.width, arrow.color);
        if (arrow.hasArrow) {
                lineRenderer_.RenderLine(tip, leftHead, arrow.width, arrow.color);
                lineRenderer_.RenderLine(tip, rightHead, arrow.width, arrow.color);
        }
}
