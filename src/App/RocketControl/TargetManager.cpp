#include "TargetManager.h"

TargetManager::TargetManager() {
    target_ = nullptr;
    reachedTimer_ = 0.0f;
}

void TargetManager::SetActiveTarget(TargetObject *target, std::function<void()> targetReachedCallback){
    targetReachedCallback_ = targetReachedCallback;

    target_ = target;
    currentTargetPos = target->transform.position;
    reachedTimer_ = 0.0f;
}

void TargetManager::CheckTargetReached(const RocketObject *rocketObject, const float dt)  {
    if (!rocketObject || !target_) {
        reachedTimer_ = 0.0f;
        return;
    }

    if (target_->IsReached())
        return;

    currentTargetPos = target_->transform.position;

    const glm::vec2 rPos = rocketObject->transform.position;
    const float dist = glm::distance(rPos, currentTargetPos);

    if (dist <= TargetObject::reachRadius) {
        reachedTimer_ += dt;

        if (reachedTimer_ >= TargetObject::reachTime) {
            // target reached
            reachedTimer_ = 0.0f;
            target_->MarkReached();
            if (targetReachedCallback_) {
                (targetReachedCallback_)();
            }
        }
    } else {
        reachedTimer_ = 0.0f;
    }
}
