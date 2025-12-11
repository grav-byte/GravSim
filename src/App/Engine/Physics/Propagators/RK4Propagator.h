#pragma once

#include "IPropagator.h"


class RK4Propagator : public IPropagator {
public:
    void Propagate(SceneObject& object, std::function<glm::vec2(const SceneObject&)> accelerationFunc, float deltaTime) override;
};
