#include "CircleVisual.h"
#include "App/Rendering//RenderingSystem.h"


CircleVisual::CircleVisual() {
    // default white color
    color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CircleVisual::CircleVisual(const glm::vec4 color){
    this->color = color;
}

CircleVisual::~CircleVisual() = default;

VisualType CircleVisual::GetType() {
    return VisualType::Circle;
}

