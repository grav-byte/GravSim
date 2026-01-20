#include "VelocityVerletPropagator.h"

void VelocityVerletPropagator::Propagate(SceneObject& object, const PhysicsContext& context, const float deltaTime) {

    // half step velocity update
    object.velocity += 0.5f * context.GetAcceleration(object) * deltaTime;
    object.transform.position += object.velocity * deltaTime;
    // another half step velocity update
    object.velocity += 0.5f * context.GetAcceleration(object) * deltaTime;

    // half step angular update
    object.angularVelocity += 0.5f * object.angularAccelerationAccumulated * deltaTime;
    object.transform.rotation += object.angularVelocity * deltaTime;
    // another half step angular update
    object.angularVelocity += 0.5f * 0.5f * object.angularAccelerationAccumulated * deltaTime;
}
