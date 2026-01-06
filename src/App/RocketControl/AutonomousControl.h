#pragma once
#include "PID/PIDController.h"
#include "RocketObject.h"


class AutonomousControl {
public:
    AutonomousControl();

    void Start() const;
    void ApplyControlInputs(RocketObject* rocketObject, float deltaTime) const;

    PIDController* GetVerticalController() const;
    PIDController* GetHorizontalController() const;
    PIDController* GetAttitudeController() const;

    bool visualizePID = false;
    float targetAltitude;

private:
    std::unique_ptr<PIDController> verticalController_;
    std::unique_ptr<PIDController> horizontalController_;
    std::unique_ptr<PIDController> attitudeController_;

    void DrawArrows(RocketObject *rocketObject) const;
};
