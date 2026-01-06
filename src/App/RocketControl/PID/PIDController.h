#pragma once
#include "PIDData.h"
#include "glm/vec3.hpp"

class PIDController {
public:
    PIDController();
    explicit PIDController(PIDData data);

    PIDData pidData;

    float Evaluate(float setpoint, float measuredValue, float derivative, float deltaTime);

    glm::vec3 GetTerms() const;

    void Reset();

private:
    float integral_ = 0.0f;
    float pTerm = 0.0f;
    float iTerm = 0.0f;
    float dTerm = 0.0f;
};
