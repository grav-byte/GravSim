//
// Created by kw on 10.12.2025.
//

#include "SemiImplicitEulerPropagator.h"

void SemiImplicitEulerPropagator::Propagate(SceneObject& object, glm::vec2 acceleration, float deltaTime) {
    object.velocity += acceleration * deltaTime;

    object.transform.position += object.velocity * deltaTime;
}