#include "PhysicsSolver.h"

#include <iostream>
#include <vector>

#include "Propagators/EulerPropagator.h"
#include "App/Engine/Scene.h"
#include "App/Layers/EngineLayer.h"
#include "Core/Application.h"
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
    timeStep_ = 1.0f / 120.0f; // 120 updates per second
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

float PhysicsSolver::GetTimeStep() const {
    return timeStep_;
}

void PhysicsSolver::StepPropagation(const Scene *scene) const {
    PhysicsContext context{ *scene, *this };

    for (SceneObject* object : scene->GetAllObjects()) {

        // propagate object
        activePropagator_->Propagate(*object, context, timeStep_);

        // update last position for verlet (only if not using verlet already)
        if (typeid(*activePropagator_) != typeid(VerletPropagator)) {
            object->lastPosition = object->transform.position - object->velocity * timeStep_;
            object->lastRotation = object->transform.rotation - object->angularVelocity * timeStep_;
        }

        if (object->mass > 0.0f) {
            // for massive objects, resolve contacts and constraints
            ContactSolver::ClearContacts(scene);

            // apply constraints
            for (const Constraint* constraint : scene->GetConstraints()) {
                constraint->ApplyConstraint(object);
            }

            ContactSolver::FindContacts(scene);
            ContactSolver::ResolveContacts(object);
        }
    }
}

glm::vec2 PhysicsSolver::GetAccelerationForObject(const Scene& scene, const SceneObject &object) const {
    auto acceleration = object.accelerationAccumulated;

    if (object.affectedByGravity) {
        acceleration += scene.globalGravity;

        for (const auto& otherObject : scene.GetAllObjects()) {
            if (otherObject->id == object.id)
                continue;
            if (otherObject->gravitates) {
                // calculate gravitational attraction
                glm::vec2 direction = otherObject->transform.position - object.transform.position;
                const float distanceSquared = glm::dot(direction, direction);
                // avoid singularity and extremely high accelerations
                if (distanceSquared < 1e-3f)
                    continue;

                constexpr float G = 1.0f;

                // TODO - pairwise force caching for better performance
                acceleration += normalize(direction) * G * otherObject->mass / distanceSquared;
            }
        }
    }
    return acceleration;
}

void PhysicsSolver::UpdatePhysics(const Scene* scene, const float deltaTime) const {
    if (!activePropagator_) {
        std::cout << "No physics propagator set!" << std::endl;
        return;
    }
    float timeAccumulator = deltaTime;

    // sub step the physics updates as often as necessary
    while (timeAccumulator >= timeStep_) {
        StepPropagation(scene);
        timeAccumulator -= timeStep_;
    }

    // reset accumulated forces
    for (const auto object : scene->GetAllObjects())
        object->ResetAccumulatedForces();

    for (const auto object : scene->GetAllObjects()) {
        object->transform.rotation = glm::mod(object->transform.rotation, 360.0f);
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
