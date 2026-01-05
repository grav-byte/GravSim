#pragma once
#include "PIDController.h"
#include "RocketObject.h"


class AutonomousControl {
public:
    AutonomousControl();
    void ApplyControlInputs(RocketObject* rocketObject, float deltaTime) const;

    void Start();

    PIDController* GetAltitudeController() const;
    float targetAltitude;

private:
    std::unique_ptr<PIDController> altitudeController_;
};
