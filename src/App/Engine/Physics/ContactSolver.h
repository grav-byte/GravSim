#pragma once
#include "App/Engine/Scene.h"
#include "App/Engine/SceneObject.h"


class ContactSolver {
public:
    static void FindContacts(const Scene *scene);
    static void ResolveContacts(SceneObject *object);

    static void ClearContacts(Scene * scene);

private:
    static void ApplyCollisionImpulse(const ContactPoint& contact);
    static void ApplyFriction(const ContactPoint& contact, float J);

    // helper functions
    static float ComputeRxN(const SceneObject* obj, const ContactPoint& contact);
    static glm::vec2 GetContactVelocityAtPoint(const ColliderBase &collider, const glm::vec2 &point);
    static float ComputeRxT (const SceneObject* obj, const ContactPoint &contact, const glm::vec2& tangent);
};