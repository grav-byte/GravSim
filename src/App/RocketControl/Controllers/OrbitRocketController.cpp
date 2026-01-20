#include "OrbitRocketController.h"

OrbitRocketController::OrbitRocketController(TargetManager *targetManager): ManualRocketController(targetManager) {
}

void OrbitRocketController::OnKeyPressed(const int keyCode) {
    ManualRocketController::OnKeyPressed(keyCode);

    if (keyCode == 'A') {
        // enable full manual mode
        facePrograde_ = true;
    } else if (keyCode == 'D') {
        facePrograde_ = false;
    }
}

void OrbitRocketController::ApplyControlInputs(RocketObject *rocketObj, float deltaTime) const {
    ManualRocketController::ApplyControlInputs(rocketObj, deltaTime);
    rocketObj->thrustAngle = 0.0f;
    const glm::vec2 direction = facePrograde_ ? rocketObj->velocity : -rocketObj->velocity;
    rocketObj->transform.rotation = glm::degrees(atan2(direction.y, direction.x)) - 90.0f;
}
