#pragma once
#include "IPropagator.h"


class VelocityVerletPropagator : public IPropagator {
public:
    void Propagate(SceneObject& object, const PhysicsContext& context, float deltaTime) override;

};
