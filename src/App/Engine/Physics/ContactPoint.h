#pragma once
#include "Colliders/ColliderBase.h"
#include "glm/vec2.hpp"

struct ContactPoint {
    ColliderBase* collider;
    ColliderBase* otherCollider;
    // world position
    glm::vec2 point;
    glm::vec2 normal;
    float penetrationDepth;

    ContactPoint(const glm::vec2 position, const glm::vec2 normal, const float penetrationDepth, ColliderBase* collider, ColliderBase* otherCollider=nullptr) {
        this->point = position;
        this->normal = normal;
        this->penetrationDepth = penetrationDepth;
        this->collider = collider;
        this->otherCollider = otherCollider;
    }
};
