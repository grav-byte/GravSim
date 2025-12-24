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

float PhysicsSolver::GetTimeStep() const {
    return timeStep_;
}

void PhysicsSolver::StepPropagation(Scene *scene) {
    currentScene_ = scene;
    for (auto& object : scene->GetAllObjects()) {

        // get acceleration function so the propagator can query it
        auto func = [this](const SceneObject& obj){ return GetAccelerationForObject(obj); };



        // propagate object
        activePropagator_->Propagate(*object, func, timeStep_);

        // update last position for verlet (only if not using verlet already)
        if (typeid(*activePropagator_) != typeid(VerletPropagator)) {
            object->lastPosition = object->transform.position - object->velocity * timeStep_;
            object->lastRotation = object->transform.rotation - object->angularVelocity * timeStep_;
        }

        // apply constraints
        for (const auto& constraint : scene->GetConstraints()) {
            constraint->ApplyConstraint(object);
        }

        // resolve contacts
        ResolveContacts(*object);

        // reset accumulated forces
        object->ResetAccumulatedForces();
    }
}

void PhysicsSolver::ResolveContacts(SceneObject &object) {
    for (const auto& contact : object.contactPoints) {
        ApplyCollisionImpulse(object, contact);
    }
    object.contactPoints.clear();
}

void PhysicsSolver::ApplyCollisionImpulse(SceneObject &obj, const ContactPoint &contact)
{
    if (contact.penetrationDepth < 0.0f) return; // no collision

    const glm::vec2 colToCon = contact.point - contact.collider->GetWorldPosition(); // vector from collider to contact
    // velocity at contact point, including rotational velocity
    const glm::vec2 contactVelocity = obj.velocity + glm::radians(obj.angularVelocity) * glm::vec2(-colToCon.y, colToCon.x);

    // relative velocity along normal
    const float normalVel = glm::dot(contactVelocity, contact.normal);

    if (normalVel > 0.0f) return; // already separating

    // compute effective mass at contact point
    const float I = 0.5f * obj.mass * obj.transform.scale.x * obj.transform.scale.x; // circle inertia
    const glm::vec2 r = contact.point - obj.transform.position; // vector from COM to contact
    // for offset colliders, include parallel axis theorem
    const float r_cross_n = r.x * contact.normal.y - r.y * contact.normal.x;
    const float invMassEffective = 1.0f / obj.mass + r_cross_n * r_cross_n / I;

    // compute impulse magnitude
    const auto otherRestitution = contact.otherCollider ? contact.otherCollider->elasticity : 1.0f;
    float J = -(1.0f + contact.collider->elasticity * otherRestitution) * normalVel / invMassEffective;

    if (normalVel > -0.1f) {
        // low-speed collision, reduce impulse to avoid jitter
        J *= 0.5f;
    }
    // apply impulse to linear and angular velocity directly
    obj.velocity += J / obj.mass * contact.normal;
    obj.angularVelocity += glm::degrees(J * r_cross_n / I);

    // positional correction to avoid sinking
    obj.transform.position += contact.normal * contact.penetrationDepth * .1f;

    // force verlet update
    obj.lastPosition = obj.transform.position;
    obj.lastRotation = obj.transform.rotation;

    // --- compute tangent friction ---
    const glm::vec2 tangent(-contact.normal.y, contact.normal.x);
    const float velAlongTangent = glm::dot(contactVelocity, tangent);

    if (fabs(velAlongTangent) > 1e-4f) { // dynamic friction
        const auto otherFriction = contact.otherCollider ? contact.otherCollider->friction : 1.0f;
        float frictionMag = contact.collider->friction * otherFriction * J;
        const float maxFriction = fabs(velAlongTangent * obj.mass);
        frictionMag = glm::min(frictionMag, maxFriction);

        glm::vec2 frictionImpulse = -frictionMag * glm::sign(velAlongTangent) * tangent;
        if (normalVel < -0.8f) {
            // high-speed collision, reduce friction to avoid jumps
            frictionImpulse *= 0.2f;
        }
        obj.velocity += frictionImpulse / obj.mass;
        obj.angularVelocity += glm::degrees((r.x * frictionImpulse.y - r.y * frictionImpulse.x) / I);
    }
}

glm::vec2 PhysicsSolver::GetAccelerationForObject(const SceneObject &object) const {
    auto acceleration = object.accelerationAccumulated;

    if (object.affectedByGravity) {
        acceleration += currentScene_->globalGravity;

        for (const auto& otherObject : currentScene_->GetAllObjects()) {
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
