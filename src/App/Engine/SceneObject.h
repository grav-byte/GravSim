//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include "ICollider.h"
#include "glm/vec2.hpp"
#include <memory>
#include <vector>


class IRenderer;

class SceneObject {
public:
    uint32_t id;
    float size;
    float mass;

    glm::vec2 position;
    glm::vec2 velocity;

    float rotation;
    float angularVelocity;

    std::unique_ptr<IRenderer> renderer;
    std::vector<std::unique_ptr<ICollider>> colliders;

    SceneObject();

};
