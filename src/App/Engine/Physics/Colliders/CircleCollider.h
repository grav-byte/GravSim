#pragma once
#include "ColliderBase.h"
#include "cereal/types/base_class.hpp"


class CircleCollider : public ColliderBase {
public:
    CircleCollider();
    explicit CircleCollider(SceneObject* parent): ColliderBase(parent) {}
    ~CircleCollider() override = default;

    ColliderType GetType() const override;
    bool CheckCollision(ColliderBase &other, ContactPoint &contact) override;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<ColliderBase>(this), localSize, localPosition, elasticity);
    }
};
