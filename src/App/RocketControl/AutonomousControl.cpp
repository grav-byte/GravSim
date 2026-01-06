#include "AutonomousControl.h"

AutonomousControl::AutonomousControl() : gravityThrust_(0), targetAltitude(0) {
    altitudeController_ = std::make_unique<PIDController>();
}

void AutonomousControl::UpdateGravityEstimate(const float deltaTime, const float currentY) {
    const float error = targetAltitude - currentY;

    constexpr float gravityLearnRate = .02f;
    gravityThrust_ += gravityLearnRate * error * deltaTime;
    std::cout << gravityThrust_ << std::endl;
    gravityThrust_ = glm::clamp(gravityThrust_, 0.0f, 1.0f);
}

void AutonomousControl::DrawArrows(RocketObject *rocketObject) {
    const auto terms = altitudeController_->GetTerms() * 5.0f; // scale for visualisation
    glm::vec2 origin = rocketObject->transform.position;
    const glm::vec2 yDir = rocketObject->transform.GetMatrix() * glm::vec4(0, 1, 0, 0);

    auto pArrow = std::make_unique<SceneObject::DebugArrow>(origin, yDir * terms.x, glm::vec4(0,0,1,1));
    auto iArrow = std::make_unique<SceneObject::DebugArrow>(origin, yDir * terms.y, glm::vec4(1,0,0,1));
    auto dArrow = std::make_unique<SceneObject::DebugArrow>(origin, yDir * terms.z, glm::vec4(0,1,0,1));
    rocketObject->debugArrows.push_back(std::move(pArrow));
    rocketObject->debugArrows.push_back(std::move(iArrow));
    rocketObject->debugArrows.push_back(std::move(dArrow));
}

void AutonomousControl::ApplyControlInputs(RocketObject *rocketObject, const float deltaTime) {
    const float currentY = rocketObject->transform.position.y;
    const float vY = rocketObject->velocity.y; // world reference frame for now

    const float yCorrection = altitudeController_->Evaluate(targetAltitude, currentY, vY, deltaTime);

    // gravity estimator
    //UpdateGravityEstimate(deltaTime, currentY);

    const float thrust = gravityThrust_ + yCorrection;
    rocketObject->thrustPercent = glm::clamp(thrust, 0.0f, 1.0f); // clamp to valid range

    DrawArrows(rocketObject);
}

void AutonomousControl::Start() {
    altitudeController_->Reset();
    gravityThrust_ = 0.0f;
}

PIDController * AutonomousControl::GetAltitudeController() const {
    if (!altitudeController_) return nullptr;
    return altitudeController_.get();
}
