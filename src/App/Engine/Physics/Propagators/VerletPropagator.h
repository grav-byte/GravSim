#pragma once
#include "IPropagator.h"

class VerletPropagator : public IPropagator {
public:
    void Propagate(SceneObject& object, const PhysicsContext& context, float deltaTime) override;

};