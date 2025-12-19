//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "SceneObject.h"

#include "../Rendering/Visuals/CircleVisual.h"
#include "Core/AppLayer.h"
#include "Physics/Colliders/CircleCollider.h"

SceneObject::SceneObject(const uint32_t objectId, const std::string& objectName)
    :
    id(objectId),
    name(objectName),
    mass(1.0f),
    velocity(0.0f, 0.0f),
    angularVelocity(0.0f),
    visual(std::make_unique<CircleVisual>()),
    lastPosition(glm::vec2(0,0)),
    affectedByGravity(true),
    gravitates(false),
    lastRotation(0)
{
    colliders = std::vector<std::unique_ptr<ColliderBase>>();
    transform = Transform();
    colliders.push_back(std::make_unique<CircleCollider>(transform));
    ResetAccumulatedForces();
}

void SceneObject::AddCollider(ColliderType type) {
    if (type == ColliderType::Circle)
        colliders.push_back(std::make_unique<CircleCollider>(transform));
}

void SceneObject::RemoveCollider(const int idx) {
    colliders.erase(colliders.begin() + idx);
}

void SceneObject::ResetAccumulatedForces() {
    accelerationAccumulated = glm::vec2(0.0f, 0.0f);
    angularAccelerationAccumulated = 0.0f;
}

void SceneObject::ApplyCollisionImpulse(const glm::vec2& colliderOffset,
                                        const glm::vec2& contactPoint,
                                        const glm::vec2& normal,
                                        float restitution)
{
    const glm::vec2 colliderPos = transform.position + colliderOffset;

    // vector from wall to collider
    const glm::vec2 penetration = colliderPos - contactPoint;
    const float depth = glm::dot(penetration, normal);

    if (depth >= 0.0f) return; // no collision


    const glm::vec2 r = colliderOffset; // vector from COM to contact
    // velocity at contact point, including rotational velocity
    const glm::vec2 contactVelocity = velocity + glm::radians(angularVelocity) * glm::vec2(-r.y, r.x);

    // relative velocity along normal
    float normalVel = glm::dot(contactVelocity, normal);

    if (normalVel > 0.0f) return; // already separating

    // compute effective mass at contact point
    const float I = 0.5f * mass * transform.scale.x * transform.scale.x; // circle inertia
    // for offset colliders, include parallel axis theorem
    const float r_cross_n = r.x * normal.y - r.y * normal.x;
    const float invMassEffective = 1.0f / mass + r_cross_n * r_cross_n / I;

    // compute impulse magnitude
    const float J = -(1.0f + restitution) * normalVel / invMassEffective;

    // apply impulse to linear and angular velocity directly
    velocity += J / mass * normal;
    angularVelocity += glm::degrees(J * r_cross_n / I);

    // positional correction to avoid sinking
    transform.position -= normal * depth;
}

void SceneObject::ApplyForce(const glm::vec2 &force, const glm::vec2 &atPoint) {
    if (mass <= 0.0f)
        return;

    // linear acceleration
    accelerationAccumulated += force / mass;

    if (atPoint != glm::vec2(0.0f, 0.0f)) {
        // moment of inertia for solid disk: I = 0.5 * m * r^2
        const float I = 0.5f * mass * transform.scale.x * transform.scale.x;

        // torque = r × F (2D cross product)
        const float torque = atPoint.x * force.y - atPoint.y * force.x;

        // angular acceleration
        angularAccelerationAccumulated += torque / I;
    }
}
