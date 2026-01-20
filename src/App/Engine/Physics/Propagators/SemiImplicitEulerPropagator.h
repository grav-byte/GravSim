#pragma once
#include "IPropagator.h"

class SemiImplicitEulerPropagator : public IPropagator {
public:
    void Propagate(SceneObject& object, const PhysicsContext& context, float deltaTime) override;

};