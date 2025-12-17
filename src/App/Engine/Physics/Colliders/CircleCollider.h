#pragma once
#include "ColliderBase.h"
#include "cereal/types/base_class.hpp"


class CircleCollider : public ColliderBase {
public:
    CircleCollider();
    explicit CircleCollider(Transform &parentTransform) : ColliderBase(parentTransform) {}
    ~CircleCollider() override = default;

    ColliderType GetType() const override;
    bool CheckCollision(const ColliderBase &other) override;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<ColliderBase>(this), size, localPosition, elasticity, parentTransform);
    }
};
