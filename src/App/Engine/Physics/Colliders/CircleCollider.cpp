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
        const glm::vec2 centerA = GetWorldPosition();
        const glm::vec2 centerB = other.GetWorldPosition();
        const glm::vec2 dir = centerA - centerB;

        const float radiusA = GetWorldSize().x;
        const float radiusB = other.GetWorldSize().x;

        const float distSq = glm::dot(dir, dir);
        const float radiusSum = radiusA + radiusB;

        const glm::vec2 normal = glm::normalize(dir);
        const glm::vec2 point = centerB + normal * radiusB;
        contact = ContactPoint(point, normal, radiusSum - glm::length(dir), this, &other);

        return distSq <= radiusSum * radiusSum;
    }

    std::cout << "CircleCollider: Unsupported collider type for collision check." << std::endl;
    return false;
}
