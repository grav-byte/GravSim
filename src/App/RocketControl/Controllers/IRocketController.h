#pragma once
#include "App/RocketControl/TargetManager.h"

class IRocketController {
public:
    explicit IRocketController(TargetManager* targetManager) : targetManager_(targetManager) {}
    virtual ~IRocketController() = default;

    virtual void Start() const = 0;
    virtual void OnKeyPressed(int keyCode) = 0;
    virtual void OnKeyReleased(int keyCode) = 0;
    virtual void ApplyControlInputs(RocketObject *rocketObj, float deltaTime) const = 0;

    TargetManager* GetTargetManager() const { return targetManager_; }

protected:
    TargetManager* targetManager_;
};
