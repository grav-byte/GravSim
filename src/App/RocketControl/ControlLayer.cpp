#include "ControlLayer.h"

#include "App/Layers/EngineLayer.h"
#include "App/Layers/UILayer.h"
#include "Controllers/ManualRocketController.h"
#include "Core/Application.h"
#include "Core/InputEvents.h"
#include "UI/RocketControllerUI.h"
#include "UI/RocketStateUI.h"


ControlLayer::ControlLayer(): rocketObj_(nullptr), engine_(nullptr) {
    targetManager_ = std::make_unique<TargetManager>();
}

void ControlLayer::OnInit() {
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();

    activeControl_ = std::make_unique<ManualRocketController>(targetManager_.get());

    if (UILayer* uiLayer = Core::Application::Get().GetLayer<UILayer>()) {
        // add uis
        uiLayer->AddUIElement(std::make_unique<RocketStateUI>());
        uiLayer->AddUIElement(std::make_unique<RocketControllerUI>());
    }
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

    activeControl_->ApplyControlInputs(rocketObj_, deltaTime);
    targetManager_->CheckTargetReached(rocketObj_, deltaTime);

    const float maxThrustAngle = rocketObj_->GetMaxThrustAngle();
    rocketObj_->thrustAngle = std::clamp(rocketObj_->thrustAngle, -maxThrustAngle, maxThrustAngle);
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
        if (rocketObj_) {
            rocketObj_->StartSound();
            activeControl_->Start();
        }
    }

    if (event.GetEventType() == Core::SimulationPaused) {
        if (rocketObj_) {
            rocketObj_->StopSound();
            rocketObj_->UpdateVisualisation();
        }
    }

    if (event.GetEventType() == Core::KeyPressed) {
        const auto keyEvent = dynamic_cast<Core::KeyPressedEvent&>(event);
        activeControl_->OnKeyPressed(keyEvent.GetKeyCode());

    }
    if (event.GetEventType() == Core::KeyReleased) {
        const auto &keyEvent = dynamic_cast<Core::KeyReleasedEvent&>(event);
        activeControl_->OnKeyReleased(keyEvent.GetKeyCode());
    }
}

RocketObject * ControlLayer::GetRocketObject() const { return rocketObj_; }

IRocketController * ControlLayer::GetActiveControl() const {
    return activeControl_.get();
}

void ControlLayer::OnRender() {
}
