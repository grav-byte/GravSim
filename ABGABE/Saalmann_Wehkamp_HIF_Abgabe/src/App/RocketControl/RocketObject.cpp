#include "RocketObject.h"

#include "App/Engine/Physics/Colliders/CircleCollider.h"
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
    // setup child objects
    auto nozzleUq = std::make_unique<SceneObject>(0, "Rocket Nozzle");
    auto exhaustUq = std::make_unique<SceneObject>(0, "Rocket Exhaust");

    // nozzle visual
    auto sprite = std::make_unique<SpriteVisual>("../assets/sprites/rocket_nozzle.png");
    nozzleUq->visual = std::move(sprite);
    nozzleUq->canFocusCamera = false;
    nozzleUq->transform.scale = glm::vec2(0.2);

    // exhaust visual
    auto shaderVisual = std::make_unique<ShaderVisual>();
    shaderVisual->shaderPath = "exhaust.frag";
    exhaustUq->canFocusCamera = false;
    exhaustShaderData_ = &shaderVisual->shaderData;
    exhaustUq->visual = std::move(shaderVisual);
    exhaustUq->transform.scale = glm::vec2(6.0, 4.0);

    // remove colliders
    exhaustUq->colliders.clear();
    nozzleUq->colliders.clear();

    // store raw pointers
    nozzleObj_ = nozzleUq.get();
    exhaustObj_ = exhaustUq.get();

    // add to scene
    nozzleId_ = scene.AddObject(std::move(nozzleUq));
    exhaustId_ = scene.AddObject(std::move(exhaustUq));

    // setup rocket object
    name = "Rocket";
    mass = 50.0f;
    visual = std::make_unique<SpriteVisual>("../assets/sprites/rocket_main.png");
    colliders.clear();
    auto col1 = std::make_unique<CircleCollider>(this);
    auto col2 = std::make_unique<CircleCollider>(this);
    auto col3 = std::make_unique<CircleCollider>(this);
    auto col4 = std::make_unique<CircleCollider>(this);
    auto col5 = std::make_unique<CircleCollider>(this);
    col1->localPosition = glm::vec2(0.0f, 0.7f);
    col2->localPosition = glm::vec2(-.35f, -.45f);
    col3->localPosition = glm::vec2(.35f, -.45f);
    col4->localPosition = glm::vec2(-.2f, -.8f);
    col5->localPosition = glm::vec2(.2f, -.8f);
    colliders.emplace_back(std::move(col1));
    colliders.emplace_back(std::move(col2));
    colliders.emplace_back(std::move(col3));
    colliders.emplace_back(std::move(col4));
    colliders.emplace_back(std::move(col5));
    for (auto& col : colliders) {
        col->localSize = glm::vec2(0.22f);
        col->elasticity = .1f;
    }
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
    if(rocketSound_) {
        auto layer = Core::Application::Get().GetLayer<AudioLayer>();
        if (layer)
            layer->StopSound(std::move(rocketSound_));
    }
}

void RocketObject::UpdateVisualisation() const {
    exhaustShaderData_->floats["uThrust"] = thrustPercent;
    AudioLayer::AdjustSoundVolume((thrustPercent + .5f) * 2.0f, rocketSound_.get());
    AudioLayer::AdjustSoundPitch(thrustPercent * .4f + .2f, rocketSound_.get());

    const glm::vec2 yDir = transform.GetMatrix() * glm::vec4(0, 1, 0, 0);
    exhaustObj_->transform.position = transform.position + yDir * exhaustOffset_;
    nozzleObj_->transform.position = transform.position + yDir * nozzleOffset_;
    exhaustObj_->transform.rotation = transform.rotation + thrustAngle;
    nozzleObj_->transform.rotation = transform.rotation + thrustAngle;
}

glm::vec2 RocketObject::GetThrustPosition() const {
    return exhaustObj_->transform.position - transform.position;
}

glm::vec2 RocketObject::GetThrustVector() const {
    const float angleRad = glm::radians(thrustAngle + transform.rotation);
    const glm::vec2 dir = { -sin(angleRad), cos(angleRad) };
    const float magnitude = thrustPercent * maxThrustForce_;
    return dir * magnitude;
}

float RocketObject::GetMaxThrustAngle() const { return maxThrustAngle_; }

float RocketObject::GetInertia() const {
    const float width = transform.scale.x;
    const float height = transform.scale.y * 2.0f;
    return 1.0f / 12.0f * mass * (width * width + height * height);
}

void RocketObject::RelinkObjects(const Scene &scene) {
    nozzleObj_ = scene.GetObjById(nozzleId_);
    exhaustObj_ = scene.GetObjById(exhaustId_);
    exhaustShaderData_ = &dynamic_cast<ShaderVisual*>(exhaustObj_->visual.get())->shaderData;
    exhaustObj_->canFocusCamera = false;
    nozzleObj_->canFocusCamera = false;

    UpdateVisualisation();
}
