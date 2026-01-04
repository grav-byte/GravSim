#pragma once
#include "RocketObject.h"


class UserControl {
public:
    void OnKeyPressed(int keyCode);
    void OnKeyReleased(int keyCode);

    void ApplyUserControl(RocketObject *rocketObj, float deltaTime) const;

    bool leftHeld_;
    bool rightHeld_;
    bool upHeld_;
    bool downHeld_;
};
