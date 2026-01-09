#pragma once
#include "RocketObject.h"
#include "Controllers/ManualRocketController.h"
#include "App/Layers/EngineLayer.h"
#include "Controllers/AutonomousPIDRocketController.h"
#include "Core/AppLayer.h"

class ControlLayer : public Core::AppLayer {
public:
    ControlLayer();

    void OnInit() override;

    void OnUpdate(float deltaTime) override;

    void FindRocket(Core::Event &event);

    void OnEvent(Core::Event &event) override;

    RocketObject* GetRocketObject() const;

    AutonomousPIDRocketController* GetAutoControl() const;

    void OnRender() override;

    bool manualControlEnabled = true;

private:
    void CreateRocket(Scene *scene);

    std::unique_ptr<ManualRocketController> userControl_;
    std::unique_ptr<AutonomousPIDRocketController> autonomousControl_;

    RocketObject* rocketObj_;
    EngineLayer * engine_;
};
