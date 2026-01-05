#include "AutonomousControl.h"

AutonomousControl::AutonomousControl() {
    altitudeController_ = std::make_unique<PIDController>();
}

void AutonomousControl::ApplyControlInputs(RocketObject *rocketObject, const float deltaTime) const {
    const float currentY = rocketObject->transform.position.y;
    const float vY = rocketObject->velocity.y; // world reference frame for now

    float yOutput = altitudeController_->Evaluate(targetAltitude, currentY, vY, deltaTime);

    rocketObject->thrustPercent = yOutput * 0.5f + 0.5f; // remap to 0 to 1
}

PIDController * AutonomousControl::GetAltitudeController() const {
    if (!altitudeController_) return nullptr;
    return altitudeController_.get();
}
