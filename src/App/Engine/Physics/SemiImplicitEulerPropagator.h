//
// Created by kw on 10.12.2025.
//

#pragma once
#include "IPropagator.h"

class SemiImplicitEulerPropagator : public IPropagator {
public:
    void Propagate(SceneObject& object, glm::vec2 acceleration, float deltaTime) override;
};