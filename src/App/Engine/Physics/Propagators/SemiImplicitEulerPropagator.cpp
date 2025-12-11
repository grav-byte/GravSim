//
// Created by kw on 10.12.2025.
//

#include "SemiImplicitEulerPropagator.h"

void SemiImplicitEulerPropagator::Propagate(SceneObject &object,
        std::function<glm::vec2(const SceneObject &)> accelerationFunc,
        float deltaTime) {
    object.velocity += accelerationFunc(object) * deltaTime;

    object.transform.position += object.velocity * deltaTime;
}