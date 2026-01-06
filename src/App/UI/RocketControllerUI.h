#pragma once
#include "FileSelector.h"
#include "IUserInterface.h"
#include "App/RocketControl/ControlLayer.h"


class RocketControllerUI : public IUserInterface {
public:
    RocketControllerUI();

    void Draw() override;

    void DrawPIDLoading(PIDController *pidController);

    void OnEvent(Core::Event &event) override;

private:
    ControlLayer* controlLayer_ = nullptr;
    FileSelector pidSelector_;
};