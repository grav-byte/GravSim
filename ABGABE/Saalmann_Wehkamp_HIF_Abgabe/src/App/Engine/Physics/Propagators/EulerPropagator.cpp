#include "EulerPropagator.h"

#include "Core/AppLayer.h"
#include "glm/ext/quaternion_geometric.hpp"

void EulerPropagator::Propagate(SceneObject& object, const PhysicsContext& context, const float deltaTime) {

    object.transform.position += object.velocity * deltaTime;
    object.velocity += context.GetAcceleration(object) * deltaTime;

    object.transform.rotation += object.angularVelocity * deltaTime;
    object.angularVelocity += object.angularAccelerationAccumulated * deltaTime;
}
