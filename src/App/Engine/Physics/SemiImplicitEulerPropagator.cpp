//
// Created by kw on 10.12.2025.
//

#include "SemiImplicitEulerPropagator.h"

void SemiImplicitEulerPropagator::Propagate(SceneObject& object, float deltaTime, float acceleration) {
    object.velocity += acceleration * deltaTime;

    object.transform.position += object.velocity * deltaTime;
}