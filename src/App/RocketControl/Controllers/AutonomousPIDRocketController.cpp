#include "AutonomousPIDRocketController.h"

#include "../PID/PIDVisualizer.h"

AutonomousPIDRocketController::AutonomousPIDRocketController() {
    verticalController_ = std::make_unique<PIDController>();
    horizontalController_ = std::make_unique<PIDController>();
    attitudeController_ = std::make_unique<PIDController>();
    attitudeController_->SetUseAngleDifference(true);
}

void AutonomousPIDRocketController::ApplyControlInputs(RocketObject *rocketObject, const float deltaTime) const {
    const float currentX = rocketObject->transform.position.x;
    const float currentY = rocketObject->transform.position.y;
    const float currentPhi = rocketObject->transform.rotation;
    const float vX = rocketObject->velocity.x; // world reference frame for now
    const float vY = rocketObject->velocity.y; // world reference frame for now
    const float vPhi = rocketObject->angularVelocity;

    // PID evaluations

    // vertical -> thrust
    float thrust = verticalController_->Evaluate(targetPos.y, currentY, vY, deltaTime);
    thrust = glm::clamp(thrust, 0.0f, 1.0f);
    rocketObject->thrustPercent =  thrust;

    // horizontal -> target angle (phi)
    float phiTarget = horizontalController_->Evaluate(targetPos.x, currentX, vX, deltaTime);
    phiTarget = -phiTarget * maxSteeringAngle;

    // attitude -> thrust angle
    const float thrustAngle = attitudeController_->Evaluate(phiTarget, currentPhi, vPhi, deltaTime);
    rocketObject->thrustAngle = -thrustAngle * rocketObject->GetMaxThrustAngle();

    // PID visualization
    if (visualizePID.x)
        PIDVisualizer::DrawVerticalArrows(rocketObject, verticalController_->GetTerms());
    if (visualizePID.y)
         PIDVisualizer::DrawHorizontalArrows(rocketObject, horizontalController_->GetTerms(), phiTarget);
    if (visualizePID.z)
         PIDVisualizer::DrawAttitudeArrows(rocketObject, attitudeController_->GetTerms());
}

void AutonomousPIDRocketController::Start() const {
    verticalController_->Reset();
}

PIDController * AutonomousPIDRocketController::GetVerticalController() const { return verticalController_.get(); }

PIDController * AutonomousPIDRocketController::GetHorizontalController() const { return horizontalController_.get(); }

PIDController * AutonomousPIDRocketController::GetAttitudeController() const { return attitudeController_.get(); }
