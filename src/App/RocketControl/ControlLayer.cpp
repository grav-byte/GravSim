#include "ControlLayer.h"

#include "App/Layers/EngineLayer.h"
#include "Core/Application.h"
#include "Core/InputEvents.h"


ControlLayer::ControlLayer(): rocketObj_(nullptr), engine_(nullptr) {}

void ControlLayer::OnInit() {
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();
    const auto scene = engine_->GetScene();

    CreateRocket(scene);
    userControl_ = std::make_unique<UserControl>();
}

void ControlLayer::CreateRocket(Scene * const scene) {
    auto obj = std::make_unique<RocketObject>(*scene);
    rocketObj_ = obj.get();
    rocketObj_->thrustPercent = 0.0f;
    scene->AddObject(std::move(obj));
    rocketObj_->UpdateVisualisation();
}

void ControlLayer::OnUpdate(const float deltaTime) {
    if (!engine_->IsRunningSimulation() || engine_->IsSimulationPaused() || !rocketObj_)
        return;

    userControl_->ApplyUserControl(rocketObj_, deltaTime);

    rocketObj_->thrustAngle = std::clamp(rocketObj_->thrustAngle, -20.0f, 20.0f);
    rocketObj_->thrustPercent = std::clamp(rocketObj_->thrustPercent, 0.0f, 1.0f);

    rocketObj_->UpdateVisualisation();
    rocketObj_->ApplyForce(rocketObj_->GetThrustVector() * 2.0f, rocketObj_->GetThrustPosition());
}

void ControlLayer::FindRocket(Core::Event &event) {
    rocketObj_ = nullptr;
    const auto sceneEvent = dynamic_cast<SceneLoadedEvent&>(event);
    for (const auto obj : sceneEvent.GetScene()->GetAllObjects()) {
        if (obj->name == "Rocket" && dynamic_cast<RocketObject*>(obj)) {
            rocketObj_ = dynamic_cast<RocketObject*>(obj);
            rocketObj_->RelinkObjects(*sceneEvent.GetScene());
            return;
        }
    }
    std::cout << "Rocket object not found in scene!" << std::endl;
}

void ControlLayer::OnEvent(Core::Event &event) {
    if (event.GetEventType() == Core::SceneLoaded) {
        FindRocket(event);
    }

    if (event.GetEventType() == Core::SimulationStarted || event.GetEventType() == Core::SimulationResumed) {
        if (rocketObj_)
            rocketObj_->StartSound();
    }

    if (event.GetEventType() == Core::SimulationPaused) {
        if (rocketObj_) {
            rocketObj_->StopSound();
            rocketObj_->UpdateVisualisation();
        }
    }

    if (event.GetEventType() == Core::KeyPressed) {
        const auto keyEvent = dynamic_cast<Core::KeyPressedEvent&>(event);
        userControl_->OnKeyPressed(keyEvent.GetKeyCode());

    }
    if (event.GetEventType() == Core::KeyReleased) {
        const auto &keyEvent = dynamic_cast<Core::KeyReleasedEvent&>(event);
        userControl_->OnKeyReleased(keyEvent.GetKeyCode());
    }
}

RocketObject * ControlLayer::GetRocketObject() const { return rocketObj_; }

void ControlLayer::OnRender() {
}
