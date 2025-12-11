//
// Created by Lorenz Saalmann on 10.12.25.
//

#include "PhysicsSolver.h"

#include <iostream>
#include <vector>

#include "Propagators/EulerPropagator.h"
#include "App/Engine/Scene.h"
#include "Core/AppLayer.h"
#include "Propagators/RK4Propagator.h"
#include "Propagators/SemiImplicitEulerPropagator.h"
#include "Propagators/VelocityVerletPropagator.h"
#include "Propagators/VerletPropagator.h"

std::vector<PhysicsSolver::PropagatorEntry> PhysicsSolver::propagators = {
    {"Euler", [] { return std::make_unique<EulerPropagator>(); }},
    {"SI Euler", [] { return std::make_unique<SemiImplicitEulerPropagator>(); }},
    {"Verlet", [] { return std::make_unique<VerletPropagator>(); }},
    {"Vel. Verlet", [] { return std::make_unique<VelocityVerletPropagator>(); }},
    {"RK4", [] { return std::make_unique<RK4Propagator>(); }},
};

PhysicsSolver::PhysicsSolver() {
    timeAccumulator_= 0;
    timeStep_ = 1.0f / 120.0f; // 120 updates per second
    currentScene_ = nullptr;
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

void PhysicsSolver::StepPropagation(Scene *scene) {
    currentScene_ = scene;
    for (const auto& object : scene->GetAllObjects()) {
        auto func = [this](const SceneObject& obj){ return GetAccelerationForObject(obj); };
        // propagate object
        activePropagator_->Propagate(*object, func, timeStep_);
    }
}

glm::vec2 PhysicsSolver::GetAccelerationForObject(const SceneObject &object) const {
    auto acceleration = glm::vec2(0.0f, 0.0f);

    if (object.affectedByGravity) {
        acceleration = currentScene_->globalGravity;

        for (const auto& otherObject : currentScene_->GetAllObjects()) {
            if (otherObject->id == object.id)
                continue;
            if (otherObject->gravitates) {
                // calculate gravitational attraction
                glm::vec2 direction = otherObject->transform.position - object.transform.position;
                const float distanceSquared = glm::dot(direction, direction);
                // avoid singularity and extremely high accelerations
                if (distanceSquared < 1e-6f)
                    continue;

                constexpr float G = 1.0f;
                acceleration += normalize(direction) * G * otherObject->mass / distanceSquared;
            }
        }
    }
    return acceleration;
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
