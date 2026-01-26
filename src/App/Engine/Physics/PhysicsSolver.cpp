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
    timeAccumulator_ = 0.0f;
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
}

float PhysicsSolver::GetTimeStep() const {
    return timeStep_;
}

void PhysicsSolver::StepPropagation(const Scene *scene) const {
    // find contacts
    ContactSolver::ClearContacts(scene);
    ContactSolver::FindContacts(scene);

    // create context for propagation
    const PhysicsContext context{ *scene, *this };

    for (SceneObject* object : scene->GetAllObjects()) {

        // propagate object
        activePropagator_->Propagate(*object, context, timeStep_);

        // update last position
        object->lastPosition = object->transform.position - object->velocity * timeStep_;
        object->lastRotation = object->transform.rotation - object->angularVelocity * timeStep_;

        if (object->mass > 0.0f) {
            // apply constraints and resolve contacts for objects with mass
            for (const Constraint* constraint : scene->GetConstraints()) {
                constraint->ApplyConstraint(object);
            }

            ContactSolver::ResolveContacts(object);
        }
    }
}

glm::vec2 PhysicsSolver::GetAccelerationForObject(const Scene& scene, const SceneObject &object) const {
    glm::vec2 acceleration = object.accelerationAccumulated;

    if (object.affectedByGravity) {
        // apply global gravity
        acceleration += scene.globalGravity;

        for (const SceneObject* otherObject : scene.GetAllObjects()) {
            if (otherObject->id == object.id)
                continue;

            // calculate gravitational attraction
            if (otherObject->gravitates) {
                glm::vec2 direction = otherObject->transform.position - object.transform.position;
                const float distanceSquared = glm::dot(direction, direction);
                // avoid singularity and extremely high accelerations
                if (distanceSquared < 1e-3f)
                    continue;

                // G is 1 for simplicity
                constexpr float G = 1.0f;

                // TODO - pairwise force caching for better performance
                acceleration += normalize(direction) * G * otherObject->mass / distanceSquared;
            }
        }
    }
    return acceleration;
}

void PhysicsSolver::UpdatePhysics(const Scene* scene, const float deltaTime) {
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
    for (const PropagatorEntry& entry : propagators) {
        names.push_back(entry.name);
    }
    static std::vector<const char*> cstrNames;
    cstrNames.clear();
    for (const std::string& name : names) {
        cstrNames.push_back(name.c_str());
    }
    return cstrNames;
}
