#pragma once
#include "App/Engine/SceneObject.h"
#include "cereal/types/base_class.hpp"

class Scene;

class TargetObject final : public SceneObject {
public:
    explicit TargetObject();

    // cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<SceneObject>(this));
    }

    void MarkReached();
    bool IsReached() const;

    static float reachRadius;
    static float reachTime;

private:
    void PlayCompletionEffect() const;

    bool reached_ = false;
};