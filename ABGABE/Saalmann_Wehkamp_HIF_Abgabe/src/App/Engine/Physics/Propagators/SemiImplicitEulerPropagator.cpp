#include "SemiImplicitEulerPropagator.h"

void SemiImplicitEulerPropagator::Propagate(SceneObject& object, const PhysicsContext& context, const float deltaTime) {

    object.velocity += context.GetAcceleration(object) * deltaTime;
    object.transform.position += object.velocity * deltaTime;

    object.angularVelocity += object.angularAccelerationAccumulated * deltaTime;
    object.transform.rotation += object.angularVelocity * deltaTime;
}