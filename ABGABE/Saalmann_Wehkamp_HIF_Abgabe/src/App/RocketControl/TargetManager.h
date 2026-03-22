#pragma once
#include "RocketObject.h"
#include "TargetObject.h"


class TargetManager {

public:
    TargetManager();
    void SetActiveTarget(TargetObject* target, std::function<void()> targetReachedCallback);
    void CheckTargetReached(const RocketObject *rocketObject, float dt);


    glm::vec2 currentTargetPos = glm::vec2(0.0f);

private:
    std::function<void()> targetReachedCallback_;
    TargetObject* target_ = nullptr;
    float reachedTimer_ = 0.0f;
};
