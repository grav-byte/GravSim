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

    glm::bvec3 visualizePID = glm::bvec3(false); // visualize PID terms via arrows

private:
    const float maxSteeringAngle = 15.0f; // degrees
    std::unique_ptr<PIDController> verticalController_;
    std::unique_ptr<PIDController> horizontalController_;
    std::unique_ptr<PIDController> attitudeController_;

    void DrawVerticalArrows(RocketObject *rocketObject) const;

    void DrawAttitudeArrows(RocketObject *rocketObject) const;

    void DrawHorizontalArrows(RocketObject *rocketObject, float targetAngle) const;
};
