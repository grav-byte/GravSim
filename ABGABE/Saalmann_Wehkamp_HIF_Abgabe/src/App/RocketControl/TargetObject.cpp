#include "TargetObject.h"

#include <thread>

#include "App/Engine/Scene.h"
#include "App/Layers/EngineLayer.h"
#include "App/Layers/AudioLayer.h"
#include "App/Rendering/Visuals/ShaderVisual.h"
#include "Core/Application.h"

float TargetObject::reachRadius = 1.0f;
float TargetObject::reachTime = 2.0f;

TargetObject::TargetObject(): SceneObject(0, "Target") {
    auto shaderVisual = std::make_unique<ShaderVisual>();
    shaderVisual->shaderPath = "target.frag";
    shaderVisual->drawOrder = -1; // draw behind other objects
    visual = std::move(shaderVisual);

    canFocusCamera = false;
    mass = 0.0f;
    velocity = glm::vec2(0.0f);
    angularVelocity = 0.0f;

    gravitates = false;
    affectedByGravity = false;

    transform.scale = glm::vec2(15.0f);
}

void TargetObject::PlayCompletionEffect() const {
    if (auto* shader = dynamic_cast<ShaderVisual*>(visual.get())) {
        shader->shaderPath = "completed.frag";
        shader->shaderData.textures ["uNoiseTex"] = "../assets/Textures/noise_tex.png";
        shader->shaderData.floats ["uTimeOffset"] = Core::Application::GetTime();
    }

    const float delay = 2.0f / Core::Application::Get().GetTimeScale();
    // schedule deletion after delay
    auto* engineLayer = Core::Application::Get().GetLayer<EngineLayer>();
    auto scene =engineLayer->GetScene();
    engineLayer->Schedule([this, scene] { scene->DeleteObject(id); }, delay);
}

void TargetObject::MarkReached() {
    reached_ = true;
    PlayCompletionEffect();

    PlayCompletionEffect();

    if (auto* audio = Core::Application::Get().GetLayer<AudioLayer>()) {
        audio->PlaySound("../assets/audio/completed.wav", 5.0);
    }
}

bool TargetObject::IsReached() const {
    return reached_;
}