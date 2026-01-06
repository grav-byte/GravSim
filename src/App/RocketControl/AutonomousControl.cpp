#include "AutonomousControl.h"

AutonomousControl::AutonomousControl() : targetAltitude(0) {
    verticalController_ = std::make_unique<PIDController>();
    horizontalController_ = std::make_unique<PIDController>();
    attitudeController_ = std::make_unique<PIDController>();
}

void AutonomousControl::DrawArrows(RocketObject *rocketObject) const {
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

void AutonomousControl::ApplyControlInputs(RocketObject *rocketObject, const float deltaTime) const {
    const float currentY = rocketObject->transform.position.y;
    const float vY = rocketObject->velocity.y; // world reference frame for now

    const float yCorrection = verticalController_->Evaluate(targetAltitude, currentY, vY, deltaTime);

    const float thrust = yCorrection;
    rocketObject->thrustPercent = glm::clamp(thrust, 0.0f, 1.0f); // clamp to valid range

    if (visualizePID)
        DrawArrows(rocketObject);
}

void AutonomousControl::Start() const {
    verticalController_->Reset();
}

PIDController * AutonomousControl::GetVerticalController() const { return verticalController_.get(); }

PIDController * AutonomousControl::GetHorizontalController() const { return horizontalController_.get(); }

PIDController * AutonomousControl::GetAttitudeController() const { return attitudeController_.get(); }
