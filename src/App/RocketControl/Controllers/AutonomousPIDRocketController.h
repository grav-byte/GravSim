#pragma once
#include "IRocketController.h"
#include "App/RocketControl/PID/PIDController.h"
#include "App/RocketControl/RocketObject.h"
#include "App/RocketControl/TargetObject.h"


class AutonomousPIDRocketController : public IRocketController {
public:
    AutonomousPIDRocketController();
    ~AutonomousPIDRocketController() override = default;

    void Start() const;
    void ApplyControlInputs(RocketObject* rocketObject, float deltaTime) override;
    void SetActiveTarget(TargetObject* target);

    TargetObject* GetActiveTarget() const { return target_; }

    PIDController* GetVerticalController() const;
    PIDController* GetHorizontalController() const;
    PIDController* GetAttitudeController() const;

    float maxSteeringAngle = 15.0f; // degrees

    glm::bvec3 visualizePID = glm::bvec3(false); // visualize PID terms via arrows

private:
    std::unique_ptr<PIDController> verticalController_;
    std::unique_ptr<PIDController> horizontalController_;
    std::unique_ptr<PIDController> attitudeController_;

    void CheckTargetReached(RocketObject *rocketObject, float dt);

    RocketObject* rocket_ = nullptr;
    TargetObject* target_ = nullptr;

    float reachedTimer_ = 0.0f;
};
