//
// Created by Lorenz Saalmann on 05.12.25.
//

#include "CircleVisual.h"

#include "../RenderingSystem.h"


CircleVisual::CircleVisual() {
    // default white color
    color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CircleVisual::CircleVisual(const glm::vec4 color){
    this->color = color;
}

CircleVisual::~CircleVisual() = default;

void CircleVisual::Render(RenderingSystem &rendering, const Transform transform) {
    rendering.RenderCircle(transform.GetMatrix(), color);
}

