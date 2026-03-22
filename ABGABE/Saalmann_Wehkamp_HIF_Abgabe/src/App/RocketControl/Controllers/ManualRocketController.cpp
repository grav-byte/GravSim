#include "ManualRocketController.h"

ManualRocketController::ManualRocketController(TargetManager* targetManager) : IRocketController(targetManager) {
    leftHeld_ = false;
    rightHeld_ = false;
    upHeld_ = false;
    downHeld_ = false;
}

void ManualRocketController::Start() const {
}

void ManualRocketController::OnKeyPressed(const int keyCode) {
    switch (keyCode) {
        case 'A':
            leftHeld_ = true;
            break;
        case 'D':
            rightHeld_ = true;
            break;
        case 340: // shift
            upHeld_ = true;
            break;
        case 341: // ctrl
            downHeld_ = true;
            break;
        default:
            break;
    }
}

void ManualRocketController::OnKeyReleased(const int keyCode) {
    switch (keyCode) {
        case 'A':
            leftHeld_ = false;
            break;
        case 'D':
            rightHeld_ = false;
            break;
        case 340: // shift
            upHeld_ = false;
            break;
        case 341: // ctrl
            downHeld_ = false;
            break;
        default:
            break;
    }
}

void ManualRocketController::ApplyControlInputs(RocketObject *rocketObj, const float deltaTime) const {
    if (upHeld_)
        rocketObj->thrustPercent += 2.0f * deltaTime;
    else if (downHeld_)
        rocketObj->thrustPercent -= 2.0f * deltaTime;

    if (rightHeld_)
        rocketObj->thrustAngle += 50.0f * deltaTime;
    else if (leftHeld_)
        rocketObj->thrustAngle -= 50.0f * deltaTime;
}