//
// Created by Lorenz Saalmann on 05.12.25.
//

#include "CircleRenderer.h"

#include "RenderingSystem.h"


CircleRenderer::CircleRenderer() {
    // default white color
    color_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CircleRenderer::CircleRenderer(glm::vec4 color)
    : color_(color) {
}

CircleRenderer::~CircleRenderer() = default;

void CircleRenderer::Render(RenderingSystem &rendering, const Transform transform) {
    rendering.RenderCircle(transform.GetMatrix(), color_);
}

