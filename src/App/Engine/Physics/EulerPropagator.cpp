//
// Created by kw on 10.12.2025.
//

#include "EulerPropagator.h"

void EulerPropagator::Propagate(SceneObject& object, float deltaTime, float acceleration) {
    object.transform.position += object.velocity * deltaTime;

    object.velocity += acceleration * deltaTime;
}