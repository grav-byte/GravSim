#pragma once
#include "IPropagator.h"


class RK4Propagator : public IPropagator {
public:
    void Propagate(SceneObject& object, glm::vec2 acceleration, float deltaTime) override;
};
