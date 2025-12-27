#include "ContactSolver.h"

#include "App/Engine/Scene.h"

void ContactSolver::FindContacts(const Scene *scene) {
    const auto& objects = scene->GetAllObjects();
    const size_t objCount = objects.size();

    for (size_t i = 0; i < objCount; ++i) {
        SceneObject& objA = *objects[i];
        for (size_t j = i + 1; j < objCount; ++j) {
            SceneObject& objB = *objects[j];

            // check collisions between all colliders of objA and objB
            for (const auto& colliderA : objA.colliders) {
                for (const auto& colliderB : objB.colliders) {
                    ContactPoint contact;
                    if (colliderA->CheckCollision(*colliderB, contact)) {
                        objA.contactPoints.push_back(contact);
                        // generate inverse contact for objB
                        ContactPoint inverse = contact;
                        inverse.collider = contact.otherCollider;
                        inverse.otherCollider = contact.collider;
                        inverse.normal = -contact.normal;

                        objB.contactPoints.push_back(inverse);
                    }
                }
            }
        }
    }
}

void ContactSolver::ResolveContacts(SceneObject* object) {
    // resolve contacts
    for (const auto& contact : object->contactPoints) {
        ApplyCollisionImpulse(contact);
    }
}

void ContactSolver::ClearContacts(Scene *scene) {
    for (SceneObject *& object : scene->GetAllObjects()) {
        object->contactPoints.clear();
    }
}

void ContactSolver::ApplyCollisionImpulse(const ContactPoint &contact)
{
    if (contact.penetrationDepth < 0.0f) return; // no collision

    SceneObject* obj = contact.collider->parentObject;
    // can be null if colliding with wall
    SceneObject* otherObj = contact.otherCollider ? contact.otherCollider->parentObject : nullptr;

    glm::vec2 contactVelocity = GetContactVelocityAtPoint(*contact.collider, contact.point);
    contactVelocity -= otherObj ? GetContactVelocityAtPoint(*contact.otherCollider, contact.point) : glm::vec2(0.0f);

    // relative velocity along normal
    const float normalVel = glm::dot(contactVelocity, contact.normal);

    if (normalVel > 0.0f) return; // already separating

    const float rxn = ComputeRxN(obj, contact);
    const float otherRxn = otherObj ? ComputeRxN(otherObj, contact) : 0.0f;

    float invMassEffective = 1.0f / obj->mass + rxn * rxn / obj->GetInertia();
    invMassEffective += otherObj ? 1.0f / otherObj->mass + otherRxn * otherRxn / otherObj->GetInertia() : 0.0f;

    // compute impulse magnitude
    const float otherRestitution = contact.otherCollider ? contact.otherCollider->elasticity : 1.0f;
    float restitution = contact.collider->elasticity * otherRestitution;

    if (normalVel > -0.1f) {
        // low-speed collision, reduce impulse to avoid jitter
        restitution = 0.0f;
    }

    const float J = -(1.0f + restitution) * normalVel / invMassEffective;

    // apply impulse to linear and angular velocity directly
    obj->velocity += J / obj->mass * contact.normal;
    obj->angularVelocity += glm::degrees(J * rxn / obj->GetInertia());

    if (otherObj) {
        otherObj->velocity -= J / otherObj->mass * contact.normal;
        otherObj->angularVelocity -= glm::degrees(J * otherRxn / otherObj->GetInertia());
        // force verlet update
        otherObj->lastPosition = otherObj->transform.position;
        otherObj->lastRotation = otherObj->transform.rotation;
        otherObj->transform.position -= contact.normal * contact.penetrationDepth * .4f;
    }

    // positional correction to avoid sinking
    obj->transform.position += contact.normal * contact.penetrationDepth * .4f;


    // force verlet update
    obj->lastPosition = obj->transform.position;
    obj->lastRotation = obj->transform.rotation;

    ApplyFriction(contact, J);
}

void ContactSolver::ApplyFriction(const ContactPoint &contact, float J) {
    const glm::vec2 contactVelocity = GetContactVelocityAtPoint(*contact.collider, contact.point);
    const auto obj = contact.collider->parentObject;
    const glm::vec2 r = contact.point - obj->transform.position;
    const float normalVel = glm::dot(contactVelocity, contact.normal);
    const glm::vec2 tangent(-contact.normal.y, contact.normal.x);
    const float velAlongTangent = glm::dot(contactVelocity, tangent);

    if (fabs(velAlongTangent) > 1e-4f) { // dynamic friction
        const auto otherFriction = contact.otherCollider ? contact.otherCollider->friction : 1.0f;
        float frictionMag = contact.collider->friction * otherFriction * J;
        const float maxFriction = fabs(velAlongTangent * obj->mass);
        frictionMag = glm::min(frictionMag, maxFriction);

        glm::vec2 frictionImpulse = -frictionMag * glm::sign(velAlongTangent) * tangent;
        if (normalVel < -0.8f) {
            // high-speed collision, reduce friction to avoid jumps
            frictionImpulse *= 0.2f;
        }
        obj->velocity += frictionImpulse / obj->mass;
        const float I = obj->GetInertia();
        obj->angularVelocity += glm::degrees((r.x * frictionImpulse.y - r.y * frictionImpulse.x) / I);
    }
}


glm::vec2 ContactSolver::GetContactVelocityAtPoint(const ColliderBase &collider, const glm::vec2 &point) {
    const glm::vec2 colToCon = point - collider.GetWorldPosition(); // vector from collider to contact
    // velocity at contact point, including rotational velocity
    const auto obj = collider.parentObject;
    return obj->velocity + glm::radians(obj->angularVelocity) * glm::vec2(-colToCon.y, colToCon.x);
}

float ContactSolver::ComputeRxN (const SceneObject* obj, const ContactPoint &contact) {
    // compute r x n
    const glm::vec2 r = contact.point - obj->transform.position; // vector from COM to contact
    return r.x * contact.normal.y - r.y * contact.normal.x;
}