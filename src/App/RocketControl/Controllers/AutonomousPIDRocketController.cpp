#include "AutonomousPIDRocketController.h"

#include "../PID/PIDVisualizer.h"


AutonomousPIDRocketController::AutonomousPIDRocketController(TargetManager *targetManager)
 : IRocketController(targetManager)  {
    verticalController_ = std::make_unique<PIDController>();
    horizontalController_ = std::make_unique<PIDController>();
    attitudeController_ = std::make_unique<PIDController>();
    attitudeController_->SetUseAngleDifference(true);
}

void AutonomousPIDRocketController::ApplyControlInputs(RocketObject *rocketObject, const float deltaTime) const {
    const float currentX = rocketObject->transform.position.x;
    const float currentY = rocketObject->transform.position.y;
    const float currentPhi = rocketObject->transform.rotation;
    const float vX = rocketObject->velocity.x;
    const float vY = rocketObject->velocity.y;
    const float vPhi = rocketObject->angularVelocity;
    // --- PID evaluations ---

    // vertical -> thrust
    float thrust = verticalController_->Evaluate(targetManager_->currentTargetPos.y, currentY, vY, deltaTime);
    const float angleRad = glm::radians(rocketObject->thrustAngle + rocketObject->transform.rotation);
    const glm::vec2 dir = { -sin(angleRad), cos(angleRad) };
    const float projection = glm::dot(glm::vec2(.0f,-1.0f), -dir);
    thrust /= glm::clamp(projection, .001f, 1.0f); // compensate for tilted thrust vector
    thrust = glm::clamp(thrust, 0.0f, 1.0f);
    rocketObject->thrustPercent = thrust;

    // horizontal -> target angle (phi)
    float phiTarget = horizontalController_->Evaluate(targetManager_->currentTargetPos.x, currentX, vX, deltaTime);
    phiTarget *= -maxSteeringAngle;

    // attitude -> thrust angle
    const float thrustAngle = attitudeController_->Evaluate(phiTarget, currentPhi, vPhi, deltaTime);
    rocketObject->thrustAngle = -thrustAngle * rocketObject->GetMaxThrustAngle();

    // --- PID visualisation ---
    if (visualizePID.x)
        PIDVisualizer::DrawVerticalArrows(rocketObject, verticalController_->GetTerms());
    if (visualizePID.y)
         PIDVisualizer::DrawHorizontalArrows(rocketObject, horizontalController_->GetTerms(), phiTarget);
    if (visualizePID.z)
         PIDVisualizer::DrawAttitudeArrows(rocketObject, attitudeController_->GetTerms());
}

void AutonomousPIDRocketController::Start() const {
    verticalController_->Reset();
    // default values:
    verticalController_->pidData.pGain = 0.5f;
    verticalController_->pidData.iGain = 0.01f;
    verticalController_->pidData.dGain = 1.0f;
    verticalController_->pidData.bias = .4f;

    horizontalController_->pidData.pGain = 0.18f;
    horizontalController_->pidData.iGain = 0.0f;
    horizontalController_->pidData.dGain = .59f;
    horizontalController_->pidData.bias = .0f;

    attitudeController_->pidData.pGain = 0.15f;
    attitudeController_->pidData.iGain = 0.0f;
    attitudeController_->pidData.dGain = .15f;
    attitudeController_->pidData.bias = 0.0f;
}

PIDController * AutonomousPIDRocketController::GetVerticalController() const { return verticalController_.get(); }

PIDController * AutonomousPIDRocketController::GetHorizontalController() const { return horizontalController_.get(); }

PIDController * AutonomousPIDRocketController::GetAttitudeController() const { return attitudeController_.get(); }

void AutonomousPIDRocketController::OnKeyPressed(int keyCode) {
}

void AutonomousPIDRocketController::OnKeyReleased(int keyCode) {
}
