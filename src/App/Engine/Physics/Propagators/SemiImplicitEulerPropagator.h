//
// Created by kw on 10.12.2025.
//

#pragma once
#include "IPropagator.h"

class SemiImplicitEulerPropagator : public IPropagator {
public:
    void Propagate(SceneObject &object, std::function<glm::vec2(const SceneObject &)> accelerationFunc,
        float deltaTime) override;
};