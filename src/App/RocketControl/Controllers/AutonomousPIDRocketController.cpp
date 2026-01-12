#include "AutonomousPIDRocketController.h"

#include "../PID/PIDVisualizer.h"


AutonomousPIDRocketController::AutonomousPIDRocketController() {
    verticalController_ = std::make_unique<PIDController>();
    horizontalController_ = std::make_unique<PIDController>();
    attitudeController_ = std::make_unique<PIDController>();
    attitudeController_->SetUseAngleDifference(true);
}

void AutonomousPIDRocketController::SetActiveTarget(TargetObject* target) {
    if (target_ == target) return;
    if (!target) return;

    target_ = target;
    reachedTimer_ = 0.0f;
}

void AutonomousPIDRocketController::ApplyControlInputs(RocketObject *rocketObject, const float deltaTime) {
    const float currentX = rocketObject->transform.position.x;
    const float currentY = rocketObject->transform.position.y;
    const float currentPhi = rocketObject->transform.rotation;
    const float vX = rocketObject->velocity.x;
    const float vY = rocketObject->velocity.y;
    const float vPhi = rocketObject->angularVelocity;
    const glm::vec2 targetPos = target_ ? target_->transform.position : glm::vec2(0.0f);
    // --- PID evaluations ---

    // vertical -> thrust
    float thrust = verticalController_->Evaluate(targetPos.y, currentY, vY, deltaTime);
    const float angleRad = glm::radians(rocketObject->thrustAngle + rocketObject->transform.rotation);
    const glm::vec2 dir = { -sin(angleRad), cos(angleRad) };
    const float projection = glm::dot(glm::vec2(.0f,-1.0f), -dir);
    thrust /= glm::clamp(projection, .001f, 1.0f); // compensate for tilted thrust vector
    thrust = glm::clamp(thrust, 0.0f, 1.0f);
    rocketObject->thrustPercent = thrust;

    // horizontal -> target angle (phi)
    float phiTarget = horizontalController_->Evaluate(targetPos.x, currentX, vX, deltaTime);
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

    CheckTargetReached(rocketObject, deltaTime);
}

void AutonomousPIDRocketController::Start() const {
    verticalController_->Reset();
}

PIDController * AutonomousPIDRocketController::GetVerticalController() const { return verticalController_.get(); }

PIDController * AutonomousPIDRocketController::GetHorizontalController() const { return horizontalController_.get(); }

PIDController * AutonomousPIDRocketController::GetAttitudeController() const { return attitudeController_.get(); }

void AutonomousPIDRocketController::CheckTargetReached(RocketObject *rocketObject, float dt) {
    if (!rocketObject || !target_) {
        reachedTimer_ = 0.0f;
        return;
    }

    if (target_->IsReached()) return;

    const glm::vec2 rPos = rocketObject->transform.position;
    const glm::vec2 tPos = target_->transform.position;
    const float dist = glm::distance(rPos, tPos);

    if (dist <= TargetObject::reachRadius) {
        reachedTimer_ += dt;

        if (reachedTimer_ >= TargetObject::reachTime) {
            reachedTimer_ = 0.0f;
            target_->MarkReached();
        }
    } else {
        reachedTimer_ = 0.0f;
    }
}