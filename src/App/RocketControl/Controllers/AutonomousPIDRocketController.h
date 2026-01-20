#pragma once
#include "IRocketController.h"
#include "App/RocketControl/PID/PIDController.h"
#include "App/RocketControl/RocketObject.h"


class AutonomousPIDRocketController : public IRocketController {
public:
    explicit AutonomousPIDRocketController(TargetManager* targetManager);
    ~AutonomousPIDRocketController() override = default;

    void Start() const override;
    void ApplyControlInputs(RocketObject* rocketObject, float deltaTime) const override;

    PIDController* GetVerticalController() const;
    PIDController* GetHorizontalController() const;
    PIDController* GetAttitudeController() const;

    void OnKeyPressed(int keyCode) override;

    void OnKeyReleased(int keyCode) override;

    float maxSteeringAngle = 15.0f; // degrees

    glm::bvec3 visualizePID = glm::bvec3(false); // visualize PID terms via arrows

private:
    std::unique_ptr<PIDController> verticalController_;
    std::unique_ptr<PIDController> horizontalController_;
    std::unique_ptr<PIDController> attitudeController_;
};
