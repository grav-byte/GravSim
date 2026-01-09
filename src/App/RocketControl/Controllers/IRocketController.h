#pragma once
#include "App/RocketControl/RocketObject.h"

class IRocketController {
public:
    virtual ~IRocketController() = default;

private:
    virtual void ApplyControlInputs(RocketObject *rocketObj, float deltaTime) const = 0;
};
