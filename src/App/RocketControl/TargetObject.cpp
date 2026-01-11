#include "TargetObject.h"

#include "App/Rendering/Visuals/ShaderVisual.h"
#include "Core/Application.h"

TargetObject::TargetObject(): SceneObject(0, "Target") {
    auto shaderVisual = std::make_unique<ShaderVisual>();
    shaderVisual->shaderPath = "target.frag";
    visual = std::move(shaderVisual);

    canFocusCamera = false;
    mass = 0.0f;
    velocity = glm::vec2(0.0f);
    angularVelocity = 0.0f;

    gravitates = false;
    affectedByGravity = false;

    transform.scale = glm::vec2(15.0f);
}

void TargetObject::PlayCompletionEffect() {
    if (auto* shader = dynamic_cast<ShaderVisual*>(visual.get())) {
        shader->shaderPath = "completed.frag";
        shader->shaderData.textures ["uNoiseTex"] = "../assets/Textures/noise_tex.png";
        shader->shaderData.floats ["uTimeOffset"] = Core::Application::GetTime();
    }
}