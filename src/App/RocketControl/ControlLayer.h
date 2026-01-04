#pragma once
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

    void OnRender() override;

private:
    void CreateRocket(Scene *scene);
    std::unique_ptr<UserControl> userControl_;
    RocketObject* rocketObj_;
    EngineLayer * engine_;
};
