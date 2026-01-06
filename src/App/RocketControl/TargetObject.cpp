#include "TargetObject.h"

#include "App/Rendering/Visuals/ShaderVisual.h"

TargetObject::TargetObject(Scene &): SceneObject(0, "Target") {
    auto shaderVisual = std::make_unique<ShaderVisual>();
    shaderVisual->shaderPath = "target.frag";
    visual = std::move(shaderVisual);

    canFocusCamera = false;
    mass = 0.0f;
    velocity = glm::vec2(0.0f);
    angularVelocity = 0.0f;

    gravitates = false;
    affectedByGravity = false;

    transform.scale = glm::vec2(6.0f, 6.0f);
}
