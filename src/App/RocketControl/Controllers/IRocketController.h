#pragma once
#include "App/RocketControl/RocketObject.h"

class IRocketController {
public:
    virtual ~IRocketController() = default;

protected:
    virtual void ApplyControlInputs(RocketObject *rocketObj, float deltaTime) = 0;
};
