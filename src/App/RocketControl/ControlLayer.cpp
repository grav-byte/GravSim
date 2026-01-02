#include "ControlLayer.h"

#include "App/Layers/EngineLayer.h"
#include "Core/Application.h"


void ControlLayer::OnInit() {
    const auto scene = Core::Application::Get().GetLayer<EngineLayer>()->GetScene();

    CreateRocket(scene);
}

void ControlLayer::CreateRocket(Scene * const scene) {
    auto obj = std::make_unique<RocketObject>(*scene);
    rocketObj_ = obj.get();
    rocketObj_->thrustAngle = 5.0f;
    rocketObj_->thrustPercent = .15f;
    scene->AddObject(std::move(obj));
}

void ControlLayer::OnUpdate(float deltaTime) {
    rocketObj_->thrustPercent = sin(Core::Application::GetTime() * .4f) * .5f + .5f;
    rocketObj_->thrustAngle = sin(Core::Application::GetTime() * .4f) * 20.0f;
    rocketObj_->UpdateVisualisation();
}

void ControlLayer::OnEvent(Core::Event &event) {
}

void ControlLayer::OnRender() {
}
