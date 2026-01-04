#include "ControlLayer.h"

#include "App/Layers/EngineLayer.h"
#include "Core/Application.h"
#include "Core/InputEvents.h"


void ControlLayer::OnInit() {
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();
    const auto scene = engine_->GetScene();

    CreateRocket(scene);
}

void ControlLayer::CreateRocket(Scene * const scene) {
    auto obj = std::make_unique<RocketObject>(*scene);
    rocketObj_ = obj.get();
    rocketObj_->thrustPercent = 0.0f;
    scene->AddObject(std::move(obj));
    rocketObj_->UpdateVisualisation();
}

void ControlLayer::OnUpdate(float deltaTime) {
    if (!engine_->IsRunningSimulation() || engine_->IsSimulationPaused() || !rocketObj_)
        return;

    if (upHeld_)
        rocketObj_->thrustPercent += 2.0f * deltaTime;
    else if (downHeld_)
        rocketObj_->thrustPercent -= 2.0f * deltaTime;

    if (rightHeld_)
        rocketObj_->thrustAngle += 50.0f * deltaTime;
    else if (leftHeld_)
        rocketObj_->thrustAngle -= 50.0f * deltaTime;

    rocketObj_->UpdateVisualisation();
    rocketObj_->thrustAngle = std::clamp(rocketObj_->thrustAngle, -20.0f, 20.0f);
    rocketObj_->thrustPercent = std::clamp(rocketObj_->thrustPercent, 0.0f, 1.0f);

    rocketObj_->ApplyForce(rocketObj_->GetThrustVector() * 2.0f, rocketObj_->GetThrustPosition());
}

void ControlLayer::FindRocket(Core::Event &event) {
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

    if (event.GetEventType() == Core::SimulationPaused || event.GetEventType() == Core::SimulationStopped) {
        if (rocketObj_)
            rocketObj_->StopSound();
    }

    if (event.GetEventType() == Core::SimulationPaused || event.GetEventType() == Core::SimulationStepped) {
        if (rocketObj_)
            rocketObj_->UpdateVisualisation();
    }

    if (event.GetEventType() == Core::KeyPressed) {
        const auto keyEvent = dynamic_cast<Core::KeyPressedEvent&>(event);
        //std::cout << "Key Pressed: " << keyEvent.GetKeyCode() << std::endl;
        switch (keyEvent.GetKeyCode()) {
            case 'A':
                leftHeld_ = true;
                break;
            case 'D':
                rightHeld_ = true;
                break;
            case 340: // shift
                upHeld_ = true;
                break;
            case 341: // ctrl
                downHeld_ = true;
                break;
            default:
                break;
        }
    }
    if (event.GetEventType() == Core::KeyReleased) {
        const auto &keyEvent = dynamic_cast<Core::KeyReleasedEvent&>(event);
        switch (keyEvent.GetKeyCode()) {
            case 'A':
                leftHeld_ = false;
                break;
            case 'D':
                rightHeld_ = false;
                break;
            case 340: // shift
                upHeld_ = false;
                break;
            case 341: // ctrl
                downHeld_ = false;
                break;
            default:
                break;
        }
    }
}

void ControlLayer::OnRender() {
}
