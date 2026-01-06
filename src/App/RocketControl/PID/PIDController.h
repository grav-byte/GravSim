#pragma once
#include "PIDData.h"

class PIDController {
public:
    PIDController();
    explicit PIDController(PIDData data);

    PIDData pidData;

    float Evaluate(float setpoint, float measuredValue, float derivative, float deltaTime);
    void Reset();

private:
    float integral_ = 0.0f;
};
