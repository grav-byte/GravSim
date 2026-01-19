#pragma once
#include "IRocketController.h"
#include "App/RocketControl/RocketObject.h"


class ManualRocketController : public IRocketController {
public:
    ~ManualRocketController() override = default;
    void OnKeyPressed(int keyCode);
    void OnKeyReleased(int keyCode);

    void ApplyControlInputs(RocketObject *rocketObj, float deltaTime) const override;

private:
    bool leftHeld_;
    bool rightHeld_;
    bool upHeld_;
    bool downHeld_;
};
