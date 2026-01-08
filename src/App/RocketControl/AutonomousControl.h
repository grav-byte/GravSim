#pragma once
#include "PID/PIDController.h"
#include "RocketObject.h"


class AutonomousControl {
public:
    AutonomousControl();

    void Start() const;
    void ApplyControlInputs(RocketObject* rocketObject, float deltaTime) const;

    PIDController* GetVerticalController() const;
    PIDController* GetHorizontalController() const;
    PIDController* GetAttitudeController() const;

    glm::vec2 targetPos = glm::vec2(0.0f); // world space target (x,y)

    bool visualizePID = false;

private:
    std::unique_ptr<PIDController> verticalController_;
    std::unique_ptr<PIDController> horizontalController_;
    std::unique_ptr<PIDController> attitudeController_;

    void DrawArrows(RocketObject *rocketObject) const;
};
