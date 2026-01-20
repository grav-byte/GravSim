#pragma once

#include "IPropagator.h"


class RK4Propagator : public IPropagator {
public:
    void Propagate(SceneObject& object, const PhysicsContext& context, float deltaTime) override;

};
