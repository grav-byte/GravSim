//
// Created by kw on 10.12.2025.
//

#include "VerletPropagator.h"

void VerletPropagator::Propagate(SceneObject &object,
        std::function<glm::vec2(const SceneObject &)> accelerationFunc,
        float deltaTime) {
    object.velocity = object.transform.position - object.lastPosition;
    object.lastPosition = object.transform.position;

    // position: x = 2*x1 - x0 + a*dt^2 = x1 + v + a*dt^2
    object.transform.position = object.transform.position + object.velocity + accelerationFunc(object) * (deltaTime * deltaTime);

}