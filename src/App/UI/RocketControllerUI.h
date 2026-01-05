#pragma once
#include "IUserInterface.h"
#include "App/RocketControl/ControlLayer.h"


class RocketControllerUI : public IUserInterface {
public:
    RocketControllerUI();

    void Draw() override;

    void OnEvent(Core::Event &event) override;

private:
    ControlLayer* controlLayer_ = nullptr;
};