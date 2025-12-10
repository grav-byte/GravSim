//
// Created by Lorenz Saalmann on 10.12.25.
//

#pragma once
#include "IPropagator.h"


class VerletPropagator: public IPropagator {
public:
    void Propagate(SceneObject &object, glm::vec2 acceleration, float deltaTime) override;
};
