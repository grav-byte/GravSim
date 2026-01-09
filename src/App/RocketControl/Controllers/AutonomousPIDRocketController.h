#pragma once
#include "IRocketController.h"
#include "App/RocketControl/PID/PIDController.h"
#include "App/RocketControl/RocketObject.h"


class AutonomousPIDRocketController : public IRocketController {
public:
    AutonomousPIDRocketController();
    ~AutonomousPIDRocketController() override = default;

    void Start() const;
    void ApplyControlInputs(RocketObject* rocketObject, float deltaTime) const override;

    PIDController* GetVerticalController() const;
    PIDController* GetHorizontalController() const;
    PIDController* GetAttitudeController() const;

    glm::vec2 targetPos = glm::vec2(0.0f); // world space target (x,y)
    float maxSteeringAngle = 15.0f; // degrees

    glm::bvec3 visualizePID = glm::bvec3(false); // visualize PID terms via arrows


private:
    std::unique_ptr<PIDController> verticalController_;
    std::unique_ptr<PIDController> horizontalController_;
    std::unique_ptr<PIDController> attitudeController_;
};
