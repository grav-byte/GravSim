#pragma once
#include "AutonomousControl.h"
#include "RocketObject.h"
#include "UserControl.h"
#include "App/Layers/EngineLayer.h"
#include "Core/AppLayer.h"
#include "Core/Application.h"


class ControlLayer : public Core::AppLayer {
public:
    ControlLayer();

    void OnInit() override;

    void OnUpdate(float deltaTime) override;

    void FindRocket(Core::Event &event);

    void OnEvent(Core::Event &event) override;

    RocketObject* GetRocketObject() const;

    AutonomousControl* GetAutoControl() const;

    void OnRender() override;

    bool manualControlEnabled = true;

private:
    void CreateRocket(Scene *scene);

    std::unique_ptr<UserControl> userControl_;
    std::unique_ptr<AutonomousControl> autonomousControl_;
    RocketObject* rocketObj_;
    EngineLayer * engine_;
};
