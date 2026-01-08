#include "AutonomousControl.h"

AutonomousControl::AutonomousControl() {
    verticalController_ = std::make_unique<PIDController>();
    horizontalController_ = std::make_unique<PIDController>();
    attitudeController_ = std::make_unique<PIDController>();
    attitudeController_->SetUseAngleDifference(true);
}

void AutonomousControl::DrawVerticalArrows(RocketObject *rocketObject) const {
    const auto terms = verticalController_->GetTerms() * 5.0f; // scale for visualisation
    glm::vec2 origin = rocketObject->transform.position;
    const glm::vec2 yDir = rocketObject->transform.GetMatrix() * glm::vec4(0, 1, 0, 0);

    auto pArrow = std::make_unique<SceneObject::DebugArrow>(origin, yDir * terms.x, glm::vec4(0,0,1,1));
    auto iArrow = std::make_unique<SceneObject::DebugArrow>(origin, yDir * terms.y, glm::vec4(1,0,0,1));
    auto dArrow = std::make_unique<SceneObject::DebugArrow>(origin, yDir * terms.z, glm::vec4(0,1,0,1));
    rocketObject->debugArrows.push_back(std::move(pArrow));
    rocketObject->debugArrows.push_back(std::move(iArrow));
    rocketObject->debugArrows.push_back(std::move(dArrow));
}

void AutonomousControl::DrawAttitudeArrows(RocketObject *rocketObject) const {
    const auto terms = -attitudeController_->GetTerms() * 1.0f; // scale for visualisation
    const glm::vec2 xDir = rocketObject->transform.GetMatrix() * glm::vec4(1, 0, 0, 0);
    const glm::vec2 yDir = rocketObject->transform.GetMatrix() * glm::vec4(0, 1, 0, 0);
    glm::vec2 origin = rocketObject->transform.position + yDir * -1.0f; // at nozzle

    auto pArrow = std::make_unique<SceneObject::DebugArrow>(origin, xDir * terms.x, glm::vec4(0,0,1,1));
    auto iArrow = std::make_unique<SceneObject::DebugArrow>(origin, xDir * terms.y, glm::vec4(1,0,0,1));
    auto dArrow = std::make_unique<SceneObject::DebugArrow>(origin, xDir * terms.z, glm::vec4(0,1,0,1));
    rocketObject->debugArrows.push_back(std::move(pArrow));
    rocketObject->debugArrows.push_back(std::move(iArrow));
    rocketObject->debugArrows.push_back(std::move(dArrow));
}

void AutonomousControl::DrawHorizontalArrows(RocketObject *rocketObject, const float targetAngle) const {
    const auto terms = horizontalController_->GetTerms() * 5.0f; // scale for visualisation
    glm::vec2 origin = rocketObject->transform.position;
    const glm::vec2 xDir = rocketObject->transform.GetMatrix() * glm::vec4(1, 0, 0, 0);

    auto pArrow = std::make_unique<SceneObject::DebugArrow>(origin, xDir * terms.x, glm::vec4(0,0,1,1));
    auto iArrow = std::make_unique<SceneObject::DebugArrow>(origin, xDir * terms.y, glm::vec4(1,0,0,1));
    auto dArrow = std::make_unique<SceneObject::DebugArrow>(origin, xDir * terms.z, glm::vec4(0,1,0,1));

    const float absoluteAngle = glm::radians(-targetAngle) + rocketObject->transform.rotation;
    const glm::vec2 targetDir = glm::vec2(sin(absoluteAngle), cos(absoluteAngle));
    auto targetLine = std::make_unique<SceneObject::DebugArrow>(origin, targetDir * 3.0f, glm::vec4(1,1,1,1));
    targetLine->dashed = true;
    targetLine->hasArrow = false;

    rocketObject->debugArrows.push_back(std::move(targetLine));
    rocketObject->debugArrows.push_back(std::move(pArrow));
    rocketObject->debugArrows.push_back(std::move(iArrow));
    rocketObject->debugArrows.push_back(std::move(dArrow));
}

void AutonomousControl::ApplyControlInputs(RocketObject *rocketObject, const float deltaTime) const {
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

    if (visualizePID.x)
        DrawVerticalArrows(rocketObject);
    if (visualizePID.y)
        DrawHorizontalArrows(rocketObject, phiTarget);
    if (visualizePID.z)
        DrawAttitudeArrows(rocketObject);
}

void AutonomousControl::Start() const {
    verticalController_->Reset();
}

PIDController * AutonomousControl::GetVerticalController() const { return verticalController_.get(); }

PIDController * AutonomousControl::GetHorizontalController() const { return horizontalController_.get(); }

PIDController * AutonomousControl::GetAttitudeController() const { return attitudeController_.get(); }
