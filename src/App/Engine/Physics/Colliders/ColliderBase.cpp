#include "ColliderBase.h"

ColliderBase::ColliderBase(): size(glm::vec2(0.0f)), localPosition(glm::vec2(0.0f)) {}

ColliderBase::ColliderBase(Transform&parentTransform):
    size(glm::vec2(parentTransform.scale)),
    localPosition(glm::vec2(0.0f)),
    parentTransform(std::shared_ptr<Transform>(&parentTransform, [](Transform*){})){}

glm::vec4 ColliderBase::GetAABB() const {
    const glm::vec2 worldPos = parentTransform->position + localPosition * parentTransform->scale;
    return {worldPos.x - size.x, worldPos.y - size.y, worldPos.x + size.x, worldPos.y + size.y};
}

glm::vec2 ColliderBase::GetWorldPosition() const {
    return parentTransform->position + localPosition * parentTransform->scale;
}

bool ColliderBase::CheckCollision(const ColliderBase &other) {
    const glm::vec4 aabbA = GetAABB();
    const glm::vec4 aabbB = other.GetAABB();

    // AABB collision check
    return !(aabbA.z < aabbB.x || aabbA.x > aabbB.z || aabbA.w < aabbB.y || aabbA.y > aabbB.w);
}
