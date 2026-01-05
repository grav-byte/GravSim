#pragma once

class PIDController {
public:
    PIDController();

    PIDController(float p, float i, float d);
    float pGain;
    float iGain;
    float dGain;

    float Evaluate(float setpoint, float measuredValue, float derivative, float deltaTime);

private:
    float integral_ = 0.0f;
};
