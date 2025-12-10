//
// Created by kw on 10.12.2025.
//

#include "EulerPropagator.h"

#include "Core/AppLayer.h"

void EulerPropagator::Propagate(SceneObject& object, glm::vec2 acceleration, float deltaTime) {
    object.transform.position += object.velocity * deltaTime;

    object.velocity += acceleration * deltaTime;
}
