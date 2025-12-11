//
// Created by kw on 10.12.2025.
//

#include "EulerPropagator.h"

#include "Core/AppLayer.h"

void EulerPropagator::Propagate(SceneObject &object, std::function<glm::vec2(const SceneObject &)> accelerationFunc, float deltaTime) {
    object.transform.position += object.velocity * deltaTime;

    object.velocity += accelerationFunc(object) * deltaTime;
}
