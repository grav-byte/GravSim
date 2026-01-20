#pragma once
#include "IRocketController.h"


class ManualRocketController : public IRocketController {
public:
    explicit ManualRocketController(TargetManager* targetManager);
    ~ManualRocketController() override = default;
    void Start() const override;
    void OnKeyPressed(int keyCode) override;
    void OnKeyReleased(int keyCode) override;

    void ApplyControlInputs(RocketObject *rocketObj, float deltaTime) const override;

private:
    bool leftHeld_;
    bool rightHeld_;
    bool upHeld_;
    bool downHeld_;
};
