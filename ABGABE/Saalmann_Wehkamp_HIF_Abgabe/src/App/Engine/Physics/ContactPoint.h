#pragma once
#include "glm/vec2.hpp"

class ColliderBase;

struct ContactPoint {
    ColliderBase* collider;
    ColliderBase* otherCollider;
    // world position
    glm::vec2 point{};
    glm::vec2 normal{};
    float penetrationDepth;

    ContactPoint(): collider(nullptr), otherCollider(nullptr), point(glm::vec2(0)), normal(glm::vec2(0)), penetrationDepth(-1.0f) {}

    ContactPoint(const glm::vec2 position, const glm::vec2 normal, const float penetrationDepth, ColliderBase* collider, ColliderBase* otherCollider=nullptr) {
        this->point = position;
        this->normal = normal;
        this->penetrationDepth = penetrationDepth;
        this->collider = collider;
        this->otherCollider = otherCollider;
    }
};
