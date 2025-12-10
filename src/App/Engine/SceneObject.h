//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include "Physics/ICollider.h"
#include "glm/vec2.hpp"
#include <memory>
#include <vector>
#include <string>

#include "Transform.h"
#include "../Rendering/Renderers/IRenderer.h"


class SceneObject {
public:
    SceneObject() : id(0), transform(Transform()), mass(0.0f), velocity(glm::vec2(0,0)),
    angularVelocity(0.0f), renderer(nullptr), lastPosition(glm::vec2(0,0)), lastRotation(0) {}

    SceneObject(uint32_t objectId, const std::string &objectName);

    uint32_t id;
    std::string name;

    Transform transform;

    float mass;
    glm::vec2 velocity;
    float angularVelocity;

    std::unique_ptr<IRenderer> renderer;
    // not yet serialized
    std::vector<std::unique_ptr<ICollider>> colliders;

    // needed for verlet
    glm::vec2 lastPosition;
    float lastRotation;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(id, name, transform, mass, velocity, angularVelocity, renderer);
    }
};


