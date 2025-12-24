//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include "Physics/Colliders/ColliderBase.h"
#include "glm/vec2.hpp"
#include <memory>
#include <vector>
#include <string>

#include "Transform.h"
#include "../Rendering/Visuals/IVisual.h"
#include "Physics/ContactPoint.h"


class SceneObject {
public:
    SceneObject();

    SceneObject(uint32_t objectId, const std::string &objectName);

    uint32_t id;
    std::string name;

    Transform transform;

    float mass;

    glm::vec2 velocity;
    float angularVelocity;

    bool gravitates;
    bool affectedByGravity;

    std::unique_ptr<IVisual> visual;
    // not yet serialized
    std::vector<std::unique_ptr<ColliderBase>> colliders;

    // needed for verlet
    glm::vec2 lastPosition;
    float lastRotation;

    void AddContactPoint(const ContactPoint &point);

    std::vector<ContactPoint> contactPoints;

    void AddCollider(ColliderType type);
    void RemoveCollider(int idx);

    glm::vec2 accelerationAccumulated;
    float angularAccelerationAccumulated;
    void ApplyForce(const glm::vec2 & force, const glm::vec2 & atPoint = glm::vec2(0, 0));
    void ApplyCollisionImpulse(ContactPoint contact);

    void ResetAccumulatedForces();

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(id, name, transform, lastPosition, mass, velocity, colliders, lastRotation, angularVelocity, visual, gravitates, affectedByGravity);
        if constexpr (Archive::is_loading::value) {
            // re-link parent transforms after loading
            for (auto& collider : colliders) {
                collider->parentTransform = std::shared_ptr<Transform>(&transform, [](Transform*){});
            }
        }
    }

};


