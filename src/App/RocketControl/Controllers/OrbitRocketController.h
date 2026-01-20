#pragma once
#include "ManualRocketController.h"


class OrbitRocketController : public ManualRocketController{
public:
    explicit OrbitRocketController(TargetManager *targetManager);

    ~OrbitRocketController() override = default;

    void OnKeyPressed(int keyCode) override;

    void ApplyControlInputs(RocketObject *rocketObj, float deltaTime) const override;
private:
    bool facePrograde_ = true;
};
