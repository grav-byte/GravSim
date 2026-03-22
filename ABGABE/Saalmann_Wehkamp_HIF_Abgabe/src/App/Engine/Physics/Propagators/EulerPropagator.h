#pragma once
#include "IPropagator.h"

class EulerPropagator : public IPropagator {
public:
    void Propagate(SceneObject& object, const PhysicsContext& context, float deltaTime) override;
};
