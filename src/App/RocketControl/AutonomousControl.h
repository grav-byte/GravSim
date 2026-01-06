#pragma once
#include "PID/PIDController.h"
#include "RocketObject.h"


class AutonomousControl {
public:
    AutonomousControl();

    void Start() const;
    void ApplyControlInputs(RocketObject* rocketObject, float deltaTime) const;

    PIDController* GetAltitudeController() const;

    bool visualizePID = false;
    float targetAltitude;

private:
    std::unique_ptr<PIDController> altitudeController_;

    void DrawArrows(RocketObject *rocketObject) const;
};
