#include "PIDController.h"

#include "glm/common.hpp"

PIDController::PIDController() : pidData(PIDData(0,0,0)) {}

PIDController::PIDController(const PIDData data) : pidData(data) {}

float PIDController::Evaluate(const float setpoint, const float measuredValue, const float derivative,
                              const float deltaTime) {
    const float error = setpoint - measuredValue;

    const float p = pidData.pGain * error;

    integral_ += error * deltaTime;
    const float iMax = 1.0f / glm::max(pidData.iGain, 0.0001f); // prevent division by zero
    integral_ = glm::clamp(integral_, -iMax, iMax);
    const float i = pidData.iGain * integral_;

    const float d = -pidData.dGain * derivative;
    const float result = p + i + d;

    return glm::clamp(result, -1.0f, 1.0f);
}

void PIDController::Reset() {
    integral_ = 0.0f;
}
