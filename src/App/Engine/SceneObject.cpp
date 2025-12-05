//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "SceneObject.h"

#include "App/Rendering/CircleRenderer.h"

SceneObject::SceneObject(uint32_t objectId, const std::string& objectName) {
    transform = Transform();
    id = objectId;
    name = objectName;
    mass = 1.0f;
    velocity = glm::vec2(0.0f, 0.0f);
    angularVelocity = 0.0f;
    renderer = std::make_unique<CircleRenderer>();
    colliders = std::vector<std::unique_ptr<ICollider>>();
}
