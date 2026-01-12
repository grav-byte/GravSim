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
    void ApplyControlInputs(RocketObject* rocketObject, float deltaTime) const override;
    void SetActiveTarget(TargetObject* target, std::function<void()> targetReachedCallback);

    void CheckTargetReached(const RocketObject *rocketObject, float dt);

    PIDController* GetVerticalController() const;
    PIDController* GetHorizontalController() const;
    PIDController* GetAttitudeController() const;

    float maxSteeringAngle = 15.0f; // degrees

    glm::bvec3 visualizePID = glm::bvec3(false); // visualize PID terms via arrows
    std::function<void()> targetReachedCallback_;
    glm::vec2 currentTargetPos_ = glm::vec2(0.0f);

private:
    std::unique_ptr<PIDController> verticalController_;
    std::unique_ptr<PIDController> horizontalController_;
    std::unique_ptr<PIDController> attitudeController_;


    RocketObject* rocket_ = nullptr;
    TargetObject* target_ = nullptr;

    float reachedTimer_ = 0.0f;
};
