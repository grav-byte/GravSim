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


class SceneObject {
public:
    SceneObject() : id(0), transform(Transform()), mass(0.0f), velocity(glm::vec2(0,0)),
    angularVelocity(0.0f), visual(nullptr), lastPosition(glm::vec2(0,0)), lastRotation(0), gravitates(false), affectedByGravity(true) {}

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

    void AddCollider(ColliderType type);
    void RemoveCollider(int idx);

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(id, name, transform, lastPosition, mass, velocity, colliders, angularVelocity, visual, gravitates, affectedByGravity);
        if constexpr (Archive::is_loading::value) {
            // re-link parent transforms after loading
            for (auto& collider : colliders) {
                collider->parentTransform = std::shared_ptr<Transform>(&transform, [](Transform*){});
            }
        }
    }

};


