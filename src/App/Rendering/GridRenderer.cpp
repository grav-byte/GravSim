#include "GridRenderer.h"

#include "App/Engine/EngineEvents.h"
#include "Core/Application.h"

void GridRenderer::OnEvent(Core::Event &event) {
    if (event.GetEventType() == Core::SceneLoaded) {
        camera_ = dynamic_cast<SceneLoadedEvent&>(event).GetScene()->GetCamera();
    }
}

void GridRenderer::RenderGrid(RenderingSystem & renderer) const {
    if (!camera_ || !showGrid) {
        return;
    }
    auto projectionMatrix = camera_->GetProjectionMatrix();

    // get world-space corners from projection
    glm::vec2 worldCorners[4];
    glm::vec4 ndcCorners[4] = {
        {-1.0f, -1.0f, 0.0f, 1.0f}, // bottom-left
        { 1.0f, -1.0f, 0.0f, 1.0f}, // bottom-right
        {-1.0f,  1.0f, 0.0f, 1.0f}, // top-left
        { 1.0f,  1.0f, 0.0f, 1.0f}  // top-right
    };

    for (int i = 0; i < 4; ++i) {
        glm::vec4 worldPos = glm::inverse(projectionMatrix) * ndcCorners[i];
        worldCorners[i] = glm::vec2(worldPos) / worldPos.w;
    }

    float left   = worldCorners[0].x;
    float right  = worldCorners[1].x;
    float bottom = worldCorners[0].y;
    float top    = worldCorners[2].y;

    // snap spacing to nearest multiple of base 2
    float baseSpacing = 2.0f; // world units
    float minSpacing = 1.0f; // world units, smallest grid line spacing
    float maxSpacing = 10000.0f; // optional upper limit

    float rawSpacing = gridSpacing_ / (5 * camera_->zoom);
    float adjustedSpacing = std::pow(baseSpacing, std::round(std::log(rawSpacing) / std::log(baseSpacing)));
    adjustedSpacing = glm::clamp(adjustedSpacing, minSpacing, maxSpacing);

    for (float x = std::floor(left / adjustedSpacing) * adjustedSpacing; x <= right; x += adjustedSpacing) {
        renderer.RenderLine(glm::vec2(x, bottom), glm::vec2(x, top), 1, gridColor_);
    }
    for (float y = std::floor(bottom / adjustedSpacing) * adjustedSpacing; y <= top; y += adjustedSpacing) {
        renderer.RenderLine(glm::vec2(left, y), glm::vec2(right, y), 1, gridColor_);
    }
}