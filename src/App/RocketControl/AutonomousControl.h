#pragma once
#include "PID/PIDController.h"
#include "RocketObject.h"


class AutonomousControl {
public:
    AutonomousControl();

    void UpdateGravityEstimate(float deltaTime, float currentY);

    void DrawArrows(RocketObject *rocketObject);

    void ApplyControlInputs(RocketObject* rocketObject, float deltaTime);

    void Start();

    PIDController* GetAltitudeController() const;

    float targetAltitude;

private:
    std::unique_ptr<PIDController> altitudeController_;

    float gravityThrust_; // estimated thrust needed to counteract gravity. acts as bias for the controller
};
