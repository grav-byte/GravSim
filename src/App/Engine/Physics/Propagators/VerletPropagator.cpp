//
// Created by kw on 10.12.2025.
//

#include "VerletPropagator.h"

void VerletPropagator::Propagate(SceneObject& object, glm::vec2 acceleration, float deltaTime) {
    if (deltaTime <= 0.0f) return;

    // position: x += v*dt + 0.5*a*dt^2
    object.transform.position += object.velocity * deltaTime + 0.5f * acceleration * (deltaTime * deltaTime);

    // velocity: v += a*dt
    object.velocity += acceleration * deltaTime;

    // rotation: theta += omega*dt
    object.transform.rotation += object.angularVelocity * deltaTime;
}