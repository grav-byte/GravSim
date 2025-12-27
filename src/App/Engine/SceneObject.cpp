#include "SceneObject.h"

#include "App/Rendering/Visuals/CircleVisual.h"
#include "Core/AppLayer.h"
#include "Physics/Colliders/CircleCollider.h"

SceneObject::SceneObject(): id(0), transform(Transform()), mass(0.0f), velocity(glm::vec2(0, 0)),
                            angularVelocity(0.0f), gravitates(false), affectedByGravity(true), visual(nullptr),
                            lastPosition(glm::vec2(0, 0)),
                            lastRotation(0), accelerationAccumulated(glm::vec2(0.0f)), angularAccelerationAccumulated(0) {
}

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
    lastRotation(0),
    accelerationAccumulated(glm::vec2(0.0f)), angularAccelerationAccumulated(0)
{
    colliders = std::vector<std::unique_ptr<ColliderBase>>();
    transform = Transform();
    colliders.push_back(std::make_unique<CircleCollider>(this));
    ResetAccumulatedForces();
}

void SceneObject::AddContactPoint(const ContactPoint &point) {
    contactPoints.push_back(point);
}

void SceneObject::AddCollider(ColliderType type) {
    if (type == ColliderType::Circle)
        colliders.push_back(std::make_unique<CircleCollider>(this));
}

float SceneObject::GetInertia() const {
    return 0.5f * mass * transform.scale.x * transform.scale.x; // circle inertia
}

void SceneObject::RemoveCollider(const int idx) {
    colliders.erase(colliders.begin() + idx);
}

void SceneObject::ResetAccumulatedForces() {
    accelerationAccumulated = glm::vec2(0.0f, 0.0f);
    angularAccelerationAccumulated = 0.0f;
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
        angularAccelerationAccumulated += glm::degrees(torque / I);
    }
}
