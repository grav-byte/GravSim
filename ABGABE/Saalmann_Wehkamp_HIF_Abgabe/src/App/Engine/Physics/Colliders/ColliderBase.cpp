#include "ColliderBase.h"
#include "App/Engine/SceneObject.h"

ColliderBase::ColliderBase(): localSize(glm::vec2(1.0f)), localPosition(glm::vec2(0.0f)) {}

ColliderBase::ColliderBase(SceneObject* parent):
    localSize(glm::vec2(parent->transform.scale)),
    localPosition(glm::vec2(0.0f)),
    parentObject(parent) {}

glm::vec4 ColliderBase::GetAABB() const { // axis aligned bounding box
    const glm::vec2 worldPos = GetWorldPosition();
    const auto size = GetWorldSize();
    return {worldPos.x - size.x, worldPos.y - size.y, worldPos.x + size.x, worldPos.y + size.y};
}

glm::vec2 ColliderBase::GetWorldPosition() const {
    const auto parentTransform = parentObject->transform;
    const glm::vec2 scaled = localPosition * parentTransform.scale;

    const float radians = glm::radians(parentTransform.rotation);
    const float cosR = cos(radians);
    const float sinR = sin(radians);

    glm::vec2 rotated;
    rotated.x = scaled.x * cosR - scaled.y * sinR;
    rotated.y = scaled.x * sinR + scaled.y * cosR;

    return parentTransform.position + rotated;
}

glm::vec2 ColliderBase::GetWorldSize() const {
    return localSize * parentObject->transform.scale;
}

glm::mat4 ColliderBase::GetTransformMatrix() const {
    auto transform = glm::mat4(1.0f);
    // position
    transform = glm::translate(transform, glm::vec3(GetWorldPosition(), 0.0f));

    transform = glm::scale(transform, glm::vec3(GetWorldSize(), 1.0f));
    return transform;
}

bool ColliderBase::CheckCollision(ColliderBase &other, ContactPoint& contact) {
    const glm::vec4 aabbA = GetAABB();
    const glm::vec4 aabbB = other.GetAABB();

    const float xOverlap = std::min(aabbA.z, aabbB.z) - std::max(aabbA.x, aabbB.x);
    const float yOverlap = std::min(aabbA.w, aabbB.w) - std::max(aabbA.y, aabbB.y);

    // AABB collision check: if either overlap is <= 0, no collision
    if (xOverlap <= 0.0f || yOverlap <= 0.0f)
        return false;

    glm::vec2 normal(0.0f);

    if (xOverlap < yOverlap) {
        // collision along x-axis
        normal.x = (aabbA.x + aabbA.z) / 2.0f < (aabbB.x + aabbB.z) / 2.0f ? -1.0f : 1.0f;
    } else {
        // collision along y-axis
        normal.y = (aabbA.y + aabbA.w) / 2.0f < (aabbB.y + aabbB.w) / 2.0f ? -1.0f : 1.0f;
    }

    glm::vec2 contactPoint;

    if (xOverlap < yOverlap) {
        // midpoint along y, edge along x
        contactPoint.x = (normal.x > 0) ? aabbB.x : aabbB.z;
        contactPoint.y = (std::max(aabbA.y, aabbB.y) + std::min(aabbA.w, aabbB.w)) / 2.0f;
    } else {
        // midpoint along x, edge along y
        contactPoint.x = (std::max(aabbA.x, aabbB.x) + std::min(aabbA.z, aabbB.z)) / 2.0f;
        contactPoint.y = normal.y > 0 ? aabbB.y : aabbB.w;
    }
    const float penetrationDepth = std::min(xOverlap, yOverlap);
    contact = ContactPoint(contactPoint, normal, penetrationDepth, this, &other);
    return true;
}
