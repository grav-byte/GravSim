#pragma once
#include "App/Engine/Transform.h"
#include "stdio.h"
#include <memory>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

enum class ColliderType { Circle };

class ColliderBase {
public:
    ColliderBase();

    explicit ColliderBase(Transform &parentTransform);
    virtual ~ColliderBase() = default;

    virtual ColliderType GetType() const = 0;
    virtual bool CheckCollision(const ColliderBase& other);

    glm::vec4 GetAABB() const;
    glm::vec2 GetWorldPosition() const;
    glm::mat4 GetTransformMatrix() const;

    float elasticity = 0.8f;
    glm::vec2 size;
    glm::vec2 localPosition;
    std::shared_ptr<Transform> parentTransform;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        // nothing to serialize in base class
    }
};
