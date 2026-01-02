#pragma once
#include "RocketObject.h"
#include "Core/AppLayer.h"


class ControlLayer : public Core::AppLayer {
public:
    void OnInit() override;

    void OnUpdate(float deltaTime) override;

    void OnEvent(Core::Event &event) override;

    void OnRender() override;

private:
    void CreateRocket(Scene *scene);

    RocketObject* rocketObj_ = nullptr;
};
