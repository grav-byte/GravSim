#include "ColliderBase.h"

ColliderBase::ColliderBase(): localSize(glm::vec2(1.0f)), localPosition(glm::vec2(0.0f)) {}

ColliderBase::ColliderBase(Transform&parentTransform):
    localSize(glm::vec2(parentTransform.scale)),
    localPosition(glm::vec2(0.0f)),
    parentTransform(std::shared_ptr<Transform>(&parentTransform, [](Transform*){})){}

glm::vec4 ColliderBase::GetAABB() const {
    const glm::vec2 worldPos = parentTransform->position + localPosition * parentTransform->scale;
    const auto size = GetWorldSize();
    return {worldPos.x - size.x, worldPos.y - size.y, worldPos.x + size.x, worldPos.y + size.y};
}

glm::vec2 ColliderBase::GetWorldPosition() const {
    const glm::vec2 scaled = localPosition * parentTransform->scale;

    const float radians = glm::radians(parentTransform->rotation);
    const float cosR = cos(radians);
    const float sinR = sin(radians);

    glm::vec2 rotated;
    rotated.x = scaled.x * cosR - scaled.y * sinR;
    rotated.y = scaled.x * sinR + scaled.y * cosR;

    return parentTransform->position + rotated;
}

glm::vec2 ColliderBase::GetWorldSize() const {
    return localSize * parentTransform->scale;
}

glm::mat4 ColliderBase::GetTransformMatrix() const {
    auto transform = glm::mat4(1.0f);
    // position
    transform = glm::translate(transform, glm::vec3(GetWorldPosition(), 0.0f));

    transform = glm::scale(transform, glm::vec3(GetWorldSize(), 1.0f));
    return transform;
}

bool ColliderBase::CheckCollision(const ColliderBase &other) {
    const glm::vec4 aabbA = GetAABB();
    const glm::vec4 aabbB = other.GetAABB();

    // AABB collision check
    return !(aabbA.z < aabbB.x || aabbA.x > aabbB.z || aabbA.w < aabbB.y || aabbA.y > aabbB.w);
}
