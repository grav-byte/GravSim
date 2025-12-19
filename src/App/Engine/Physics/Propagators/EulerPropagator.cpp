//
// Created by kw on 10.12.2025.
//

#include "EulerPropagator.h"

#include "Core/AppLayer.h"
#include "glm/ext/quaternion_geometric.hpp"

void EulerPropagator::Propagate(SceneObject &object, std::function<glm::vec2(const SceneObject &)> accelerationFunc, float deltaTime) {
    object.transform.position += object.velocity * deltaTime;
    object.velocity += accelerationFunc(object) * deltaTime;

    object.transform.rotation += object.angularVelocity * deltaTime;
    object.angularVelocity += object.angularAccelerationAccumulated * deltaTime;
}
