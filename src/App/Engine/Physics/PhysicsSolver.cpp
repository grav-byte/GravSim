//
// Created by Lorenz Saalmann on 10.12.25.
//

#include "PhysicsSolver.h"

#include <iostream>

#include "EulerPropagator.h"
#include "App/Engine/Scene.h"

PhysicsSolver::PhysicsSolver() {
    globalGravity_ = glm::vec2(0.0f, -9.81f);
    propagator_ = std::make_unique<EulerPropagator>();
}

void PhysicsSolver::UpdatePhysics(Scene* scene, float deltaTime) const {
    if (!propagator_) {
        std::cout << "No physics propagator set!" << std::endl;
        return;
    }

    for (auto& object : scene->GetAllObjects()) {
        glm::vec2 acceleration = globalGravity_;
        propagator_->Propagate(*object, acceleration, deltaTime);
    }
}
