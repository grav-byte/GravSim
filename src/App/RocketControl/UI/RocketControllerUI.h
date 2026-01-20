#pragma once
#include "App/UI/FileSelector.h"
#include "App/RocketControl/PID/PIDController.h"
#include "App/UI/IUserInterface.h"
#include "App/Layers/EngineLayer.h"
#include <limits>

#include "App/RocketControl/ControlLayer.h"
#include "App/RocketControl/Controllers/AutonomousPIDRocketController.h"
#include "App/RocketControl/UI/TargetUI.h"


class RocketControllerUI : public IUserInterface {
public:
    RocketControllerUI();

    void Draw() override;

    void OnEvent(Core::Event &event) override;

private:
    int currentMode_ = 0;

    ControlLayer* controlLayer_ = nullptr;
    EngineLayer* engineLayer_ = nullptr;
    PIDController* activePID;
    TargetUI targetUI_;

    FileSelector pidFileSelector_;

    std::string saveName;

    void DrawPIDSettings(AutonomousPIDRocketController *autoCtrl);
    void DrawPIDLoading(PIDController *(&pids)[3], float &steeringAngle);

    void SaveConfig(const AutonomousPIDRocketController *autoCtrl) const;
};
