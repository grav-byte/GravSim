#include "GridRenderer.h"

#include "LineRenderer.h"
#include "App/Engine/EngineEvents.h"
#include "Core/Application.h"

GridRenderer::GridRenderer(const LineRenderer &renderer, const RenderingSystem* renderingSystem):
BaseRenderer(renderingSystem), lineRenderer_(renderer) {}

void GridRenderer::RenderGrid(const glm::vec4 &gridColor, float gridSpacing) const {
    const auto activeCamera = renderingSys_->GetActiveCamera();

    if (!activeCamera) {
        return;
    }
    const auto projectionMatrix = activeCamera->GetProjectionMatrix();

    // get world-space corners from projection
    glm::vec2 worldCorners[4];
    constexpr glm::vec4 ndcCorners[4] = {
        {-1.0f, -1.0f, 0.0f, 1.0f}, // bottom-left
        { 1.0f, -1.0f, 0.0f, 1.0f}, // bottom-right
        {-1.0f,  1.0f, 0.0f, 1.0f}, // top-left
        { 1.0f,  1.0f, 0.0f, 1.0f}  // top-right
    };

    for (int i = 0; i < 4; ++i) {
        glm::vec4 worldPos = glm::inverse(projectionMatrix) * ndcCorners[i];
        worldCorners[i] = glm::vec2(worldPos) / worldPos.w;
    }

    const float left   = worldCorners[0].x;
    const float right  = worldCorners[1].x;
    const float bottom = worldCorners[0].y;
    const float top    = worldCorners[2].y;

    // snap spacing to nearest multiple of base 2
    constexpr float baseSpacing = 2.0f; // world units
    constexpr float minSpacing = 1.0f; // world units, smallest grid line spacing

    const float rawSpacing = gridSpacing / (5 * activeCamera->zoom);
    float adjustedSpacing = std::pow(baseSpacing, std::round(std::log(rawSpacing) / std::log(baseSpacing)));
    adjustedSpacing = glm::clamp(adjustedSpacing, minSpacing, 10000.0f);

    for (float x = std::floor(left / adjustedSpacing) * adjustedSpacing; x <= right; x += adjustedSpacing) {
        lineRenderer_.RenderLine(glm::vec2(x, bottom), glm::vec2(x, top), 1, gridColor);
    }
    for (float y = std::floor(bottom / adjustedSpacing) * adjustedSpacing; y <= top; y += adjustedSpacing) {
        lineRenderer_.RenderLine(glm::vec2(left, y), glm::vec2(right, y), 1, gridColor);
    }
}
