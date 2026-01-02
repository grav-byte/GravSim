#include "RocketObject.h"

#include "App/Layers/AudioLayer.h"
#include "App/Rendering/Visuals/ShaderVisual.h"
#include "App/Rendering/Visuals/SpriteVisual.h"
#include "Core/Application.h"

RocketObject::RocketObject(Scene& scene) {
    // setup objects
    auto nozzleUq = std::make_unique<SceneObject>(0, "Rocket Nozzle");
    auto exhaustUq = std::make_unique<SceneObject>(0, "Rocket Exhaust");

    auto sprite = std::make_unique<SpriteVisual>("../assets/sprites/rocket_nozzle.png");
    nozzleUq->visual = std::move(sprite);
    nozzleUq->transform.scale = glm::vec2(0.2);

    auto shaderVisual = std::make_unique<ShaderVisual>();
    shaderVisual->shaderPath = "exhaust.frag";
    exhaustShaderData_ = &shaderVisual->shaderData;
    exhaustUq->visual = std::move(shaderVisual);
    exhaustUq->transform.scale = glm::vec2(6.0, 4.0);

    nozzleObj_ = nozzleUq.get();
    exhaustObj_ = exhaustUq.get();

    scene.AddObject(std::move(nozzleUq));
    scene.AddObject(std::move(exhaustUq));

    name = "Rocket";
    visual = std::make_unique<SpriteVisual>("../assets/sprites/rocket_main.png");

    // audio
    auto* audioLayer = Core::Application::Get().GetLayer<AudioLayer>();
    rocketSound_ = audioLayer->PlaySoundRepeating("../assets/audio/rocket_engine.wav");
}

void RocketObject::UpdateVisualisation() const {
    exhaustShaderData_->floats["uThrust"] = thrustPercent;
    AudioLayer::AdjustSoundVolume((thrustPercent + .5f) * 2.0f, rocketSound_);
    AudioLayer::AdjustSoundPitch(thrustPercent * .4f + .2f, rocketSound_);

    const glm::vec2 yDir = transform.GetMatrix() * glm::vec4(0, 1, 0, 0);
    exhaustObj_->transform.position = transform.position + yDir * exhaustOffset;
    nozzleObj_->transform.position = transform.position + yDir * nozzleOffset;
    exhaustObj_->transform.rotation = transform.rotation + thrustAngle;
    nozzleObj_->transform.rotation = transform.rotation + thrustAngle;
}
