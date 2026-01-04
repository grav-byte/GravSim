#include "RocketObject.h"

#include "App/Layers/AudioLayer.h"
#include "App/Rendering/Visuals/ShaderVisual.h"
#include "App/Rendering/Visuals/SpriteVisual.h"
#include "Core/Application.h"

RocketObject::RocketObject() : nozzleObj_(nullptr), exhaustObj_(nullptr), exhaustShaderData_(nullptr), exhaustId_(0),
                               nozzleId_(0),
                               rocketSound_(nullptr) {
    thrustPercent = 0.0f;
    thrustAngle = 0.0f;
}

RocketObject::RocketObject(Scene& scene) {
    // setup objects
    auto nozzleUq = std::make_unique<SceneObject>(0, "Rocket Nozzle");
    auto exhaustUq = std::make_unique<SceneObject>(0, "Rocket Exhaust");

    auto sprite = std::make_unique<SpriteVisual>("../assets/sprites/rocket_nozzle.png");
    nozzleUq->visual = std::move(sprite);
    nozzleUq->canFocusCamera = false;
    nozzleUq->transform.scale = glm::vec2(0.2);

    auto shaderVisual = std::make_unique<ShaderVisual>();
    shaderVisual->shaderPath = "exhaust.frag";
    exhaustUq->canFocusCamera = false;
    exhaustShaderData_ = &shaderVisual->shaderData;
    exhaustUq->visual = std::move(shaderVisual);
    exhaustUq->transform.scale = glm::vec2(6.0, 4.0);

    nozzleObj_ = nozzleUq.get();
    exhaustObj_ = exhaustUq.get();

    nozzleId_ = scene.AddObject(std::move(nozzleUq));
    exhaustId_ = scene.AddObject(std::move(exhaustUq));

    name = "Rocket";
    mass = 50.0f;
    visual = std::make_unique<SpriteVisual>("../assets/sprites/rocket_main.png");
    rocketSound_ = nullptr;
}

RocketObject::~RocketObject() {
    StopSound();
}

void RocketObject::StartSound() {
    if (rocketSound_)
        StopSound();
    auto* audioLayer = Core::Application::Get().GetLayer<AudioLayer>();
    rocketSound_ = audioLayer->PlaySoundRepeating("../assets/audio/rocket_engine.wav");
}

void RocketObject::StopSound() {
    if (rocketSound_) {
        AudioLayer::StopSound(rocketSound_);
    }
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

glm::vec2 RocketObject::GetThrustPosition() const {
    return exhaustObj_->transform.position - transform.position;
}

glm::vec2 RocketObject::GetThrustVector() const {
    const float angleRad = glm::radians(thrustAngle + transform.rotation);
    const glm::vec2 dir = { -sin(angleRad), cos(angleRad) };
    const float magnitude = thrustPercent * maxTrustForce_;
    // std::cout << "dir: " << dir.x << " ," << dir.y << std::endl;
    return dir * magnitude;
}

void RocketObject::RelinkObjects(const Scene &scene) {
    nozzleObj_ = scene.GetObjById(nozzleId_);
    exhaustObj_ = scene.GetObjById(exhaustId_);
    exhaustShaderData_ = &dynamic_cast<ShaderVisual*>(exhaustObj_->visual.get())->shaderData;
    exhaustObj_->canFocusCamera = false;
    nozzleObj_->canFocusCamera = false;

    UpdateVisualisation();
}
