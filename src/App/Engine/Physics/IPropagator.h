//
// Created by Lorenz Saalmann on 10.12.25.
//

#pragma once
#include "App/Engine/SceneObject.h"

class IPropagator {
public:
    virtual ~IPropagator() = default;

    virtual void Propagate(SceneObject& object, glm::vec2 acceleration, float deltaTime) = 0;
};
