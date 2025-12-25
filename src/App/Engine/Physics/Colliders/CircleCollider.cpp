#include "CircleCollider.h"
#include <iostream>

#include "App/Engine/SceneObject.h"

CircleCollider::CircleCollider() = default;

ColliderType CircleCollider::GetType() const {
    return ColliderType::Circle;
}

bool CircleCollider::CheckCollision(ColliderBase &other, ContactPoint &contact) {
    // AABB check
    if (!ColliderBase::CheckCollision(other, contact))
        return false;

    if (other.GetType() == ColliderType::Circle) {
        // collision with another circle
        const auto parentTransform = parentObject->transform;


        const glm::vec2 centerA = GetWorldPosition();
        const glm::vec2 centerB = other.GetWorldPosition();

        const float radiusA = GetWorldSize().x * parentTransform.scale.x;
        const float radiusB = other.GetWorldSize().x * other.parentObject->transform.scale.x;

        const float distSq = glm::dot(centerA - centerB, centerA - centerB);
        const float radiusSum = radiusA + radiusB;

        return distSq <= radiusSum * radiusSum;
    }

    std::cout << "CircleCollider: Unsupported collider type for collision check." << std::endl;
    return false;
}
