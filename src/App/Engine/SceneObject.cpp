//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "SceneObject.h"

#include "../Rendering/Renderers/CircleRenderer.h"
#include "Core/AppLayer.h"
#include "Physics/Colliders/CircleCollider.h"

SceneObject::SceneObject(const uint32_t objectId, const std::string& objectName)
    :
    id(objectId),
    name(objectName),
    mass(1.0f),
    velocity(0.0f, 0.0f),
    angularVelocity(0.0f),
    renderer(std::make_unique<CircleRenderer>()),
    lastPosition(glm::vec2(0,0)),
    affectedByGravity(true),
    gravitates(false),
    lastRotation(0)
{
    colliders = std::vector<std::unique_ptr<ColliderBase>>();
    transform = Transform();
    colliders.push_back(std::make_unique<CircleCollider>(transform));
}
