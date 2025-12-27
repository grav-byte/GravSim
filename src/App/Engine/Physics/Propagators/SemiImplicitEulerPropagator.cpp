#include "SemiImplicitEulerPropagator.h"

void SemiImplicitEulerPropagator::Propagate(SceneObject &object,
        const std::function<glm::vec2(const SceneObject &)> accelerationFunc,
        const float deltaTime) {

    object.velocity += accelerationFunc(object) * deltaTime;
    object.transform.position += object.velocity * deltaTime;

    object.angularVelocity += object.angularAccelerationAccumulated * deltaTime;
    object.transform.rotation += object.angularVelocity * deltaTime;
}