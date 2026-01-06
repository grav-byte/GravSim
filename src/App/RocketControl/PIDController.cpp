#include "PIDController.h"

#include "glm/common.hpp"

PIDController::PIDController() {
    pGain = 0.0f;
    iGain = 0.0f;
    dGain = 0.0f;
}

PIDController::PIDController(const float p, const float i, const float d) {
    pGain = p;
    iGain = i;
    dGain = d;
}

float PIDController::Evaluate(const float setpoint, const float measuredValue, const float derivative,
                              const float deltaTime) {
    const float error = setpoint - measuredValue;

    const float p = pGain * error;

    integral_ += error * deltaTime;
    const float iMax = 1.0f / glm::max(iGain, 0.0001f); // prevent division by zero
    integral_ = glm::clamp(integral_, -iMax, iMax);
    const float i = iGain * integral_;

    const float d = -dGain * derivative;
    const float result = p + i + d;

    return glm::clamp(result, -1.0f, 1.0f);
}

void PIDController::Reset() {
    integral_ = 0.0f;
}
