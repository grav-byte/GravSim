//
// Created by Lorenz Saalmann on 10.12.25.
//

#pragma once
#include "IPropagator.h"
#include "App/Engine/Scene.h"


class PhysicsSolver {
public:
    PhysicsSolver();
    void UpdatePhysics(Scene *scene, float deltaTime) const;
private:
    glm::vec2 globalGravity_;
    std::unique_ptr<IPropagator> propagator_;
};
