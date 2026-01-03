#pragma once
#include "RocketObject.h"
#include "App/Layers/EngineLayer.h"
#include "Core/AppLayer.h"
#include "Core/Application.h"


class ControlLayer : public Core::AppLayer {
public:
    void OnInit() override;

    void OnUpdate(float deltaTime) override;

    void FindRocket(Core::Event &event);

    void OnEvent(Core::Event &event) override;

    void OnRender() override;

private:
    void CreateRocket(Scene *scene);

    bool leftHeld_ = false;
    bool rightHeld_ = false;
    bool upHeld_ = false;
    bool downHeld_ = false;
    RocketObject* rocketObj_ = nullptr;
    EngineLayer * engine_;
};
