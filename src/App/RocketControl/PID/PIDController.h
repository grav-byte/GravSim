#pragma once
#include "PIDData.h"
#include "glm/vec3.hpp"

class PIDController {
public:
    PIDController();
    explicit PIDController(PIDData data);

    PIDData pidData;

    float Evaluate(float setpoint, float measuredValue, float derivative, float deltaTime);

    void SetUseAngleDifference(bool useAngleDifference);

    glm::vec3 GetTerms() const;

    void Reset();

private:
    bool useAngleDifference_ = false;
    float integral_ = 0.0f;
    float pTerm_ = 0.0f;
    float iTerm_ = 0.0f;
    float dTerm_ = 0.0f;
};
