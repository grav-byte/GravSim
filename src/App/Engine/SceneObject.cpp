//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "SceneObject.h"

#include "../Rendering/Renderers/CircleRenderer.h"
#include "../Rendering/Renderers/SpriteRenderer.h"

SceneObject::SceneObject(const uint32_t objectId, const std::string& objectName)
    : id(objectId),
      name(objectName),
      transform(),
      mass(1.0f),
      velocity(0.0f, 0.0f),
      angularVelocity(0.0f),
      renderer(std::make_unique<SpriteRenderer>("../assets/sprites/rocket_main.png")),
      colliders()
{
}