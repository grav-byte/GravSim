//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include "ICollider.h"
#include "glm/vec2.hpp"
#include <memory>
#include <vector>

#include "Transform.h"
#include "App/Rendering/IRenderer.h"


class SceneObject {
public:
    SceneObject();

    uint32_t id;

    Transform transform;

    float mass;
    glm::vec2 velocity;
    float angularVelocity;

    std::unique_ptr<IRenderer> renderer;
    std::vector<std::unique_ptr<ICollider>> colliders;
};
