//
// Created by Lorenz Saalmann on 10.12.25.
//

#include "PhysicsSolver.h"

#include <iostream>
#include <vector>

#include "Propagators/EulerPropagator.h"
#include "App/Engine/Scene.h"
#include "Propagators/RK4Propagator.h"
#include "Propagators/VerletPropagator.h"

std::vector<PhysicsSolver::PropagatorEntry> PhysicsSolver::propagators = {
    {"Euler", [] { return std::make_unique<EulerPropagator>(); }},
    {"SI Euler", [] { return std::make_unique<EulerPropagator>(); }},
    {"Verlet", [] { return std::make_unique<VerletPropagator>(); }},
    {"RK4", [] { return std::make_unique<RK4Propagator>(); }},
};

PhysicsSolver::PhysicsSolver() {
    timeAccumulator_= 0;
    timeStep_ = 1.0f / 120.0f; // 120 updates per second
    globalGravity_ = glm::vec2(0.0f, -9.81f);

    activePropagator_ = std::make_unique<EulerPropagator>();
}

void PhysicsSolver::SetActivePropagator(const std::string &name) {
    for (const auto &entry : propagators) {
        if (entry.name == name) {
            activePropagator_ = entry.factory();
            return;
        }
    }
    std::cout << "Propagator with name " << name << " not found!" << std::endl;
    activePropagator_ = nullptr;
}

IPropagator * PhysicsSolver::GetActivePropagator()
    const { return activePropagator_.get(); }

void PhysicsSolver::SetTimeStep(const float timeStep) {
    timeStep_ = timeStep;
    std::cout << "Set physics time step to " << timeStep_ << " seconds." << std::endl;
}

void PhysicsSolver::StepPropagation(const Scene *scene) const {
    for (const auto& object : scene->GetAllObjects()) {
        // apply forces
        glm::vec2 acceleration = globalGravity_;

        // propagate object
        activePropagator_->Propagate(*object, acceleration, timeStep_);
    }
}

void PhysicsSolver::UpdatePhysics(Scene* scene, float deltaTime) {
    if (!activePropagator_) {
        std::cout << "No physics propagator set!" << std::endl;
        return;
    }

    timeAccumulator_ += deltaTime;

    // sub step the physics updates as often as necessary
    while (timeAccumulator_ >= timeStep_) {
        StepPropagation(scene);
        timeAccumulator_ -= timeStep_;
    }
}

std::vector<const char *> PhysicsSolver::GetPropagatorNames() {
    static std::vector<std::string> names;
    names.clear();
    for (const auto& entry : propagators) {
        names.push_back(entry.name);
    }
    static std::vector<const char*> cstrNames;
    cstrNames.clear();
    for (const auto& name : names) {
        cstrNames.push_back(name.c_str());
    }
    return cstrNames;
}
