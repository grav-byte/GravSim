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
    {"Semi Implicit Euler", [] { return std::make_unique<EulerPropagator>(); }},
    {"Verlet", [] { return std::make_unique<VerletPropagator>(); }},
    {"RK4", [] { return std::make_unique<RK4Propagator>(); }},
};

PhysicsSolver::PhysicsSolver() {
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

void PhysicsSolver::UpdatePhysics(Scene* scene, float deltaTime) const {
    if (!activePropagator_) {
        std::cout << "No physics propagator set!" << std::endl;
        return;
    }

    for (auto& object : scene->GetAllObjects()) {
        glm::vec2 acceleration = globalGravity_;
        activePropagator_->Propagate(*object, acceleration, deltaTime);
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
