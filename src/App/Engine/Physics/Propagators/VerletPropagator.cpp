#include "VerletPropagator.h"

void VerletPropagator::Propagate(SceneObject& object, const PhysicsContext& context, const float deltaTime) {

    if (object.lastPosition != object.transform.position)
        object.velocity = (object.transform.position - object.lastPosition) / deltaTime; // use verlet if there is a valid last pos

    object.lastPosition = object.transform.position;

    // position: x = 2*x1 - x0 + a*dt^2 = x1 + v + a*dt^2
    object.transform.position = object.transform.position + object.velocity * deltaTime + context.GetAcceleration(object) * (deltaTime * deltaTime);

    // update angular motion - similar to position
    if (object.lastRotation != object.transform.rotation)
        object.angularVelocity = (object.transform.rotation - object.lastRotation) / deltaTime;

    object.lastRotation = object.transform.rotation;
    object.transform.rotation = object.transform.rotation + object.angularVelocity * deltaTime + object.angularAccelerationAccumulated * (deltaTime * deltaTime);
}