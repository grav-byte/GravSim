#include "PIDController.h"

#include "glm/common.hpp"

PIDController::PIDController() : pidData(PIDData(0,0,0, 0)) {}

PIDController::PIDController(const PIDData &data) : pidData(data) {}

float PIDController::Evaluate(const float setpoint, const float measuredValue, const float derivative, const float deltaTime) {
    // compute error
    float error = setpoint - measuredValue;

    // handle angle wrapping
    if (useAngleDifference_) {
        float angleError = setpoint - measuredValue;
        // wrap to [-180, 180]
        while (angleError > 180.0f) angleError -= 360.0f;
        while (angleError < -180.0f) angleError += 360.0f;
        error = angleError;
    }

    // P
    pTerm_ = pidData.pGain * error;

    // I
    integral_ += error * deltaTime;
    const float iMax = 1.0f / glm::max(pidData.iGain, 0.0001f); // prevent division by zero
    integral_ = glm::clamp(integral_, -iMax, iMax); // anti-windup (saturation)
    iTerm_ = pidData.iGain * integral_;

    // D
    dTerm_ = -pidData.dGain * derivative;
    // Bias (Offset)
    biasTerm_ = pidData.bias;

    // sum
    const float result = pTerm_ + iTerm_ + dTerm_ + biasTerm_;
    // clamp output
    return glm::clamp(result, -1.0f, 1.0f);
}

void PIDController::SetUseAngleDifference(const bool useAngleDifference) { useAngleDifference_ = useAngleDifference; }

glm::vec4 PIDController::GetTerms() const {
    return {pTerm_, iTerm_, dTerm_, biasTerm_};
}


void PIDController::Reset() {
    integral_ = 0.0f;
}
