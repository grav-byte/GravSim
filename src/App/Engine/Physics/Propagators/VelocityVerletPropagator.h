#pragma once
#include "IPropagator.h"


class VelocityVerletPropagator : public IPropagator {
public:
    void Propagate(SceneObject &object, std::function<glm::vec2(const SceneObject &)> accelerationFunc,
        float deltaTime) override;
};
