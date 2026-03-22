#pragma once
#include "IUserInterface.h"
#include "App/Layers/EngineLayer.h"


class FollowingUI : public IUserInterface {
public:
    FollowingUI();
    void OnEvent(Core::Event &event) override;

    void Draw() override;
private:
    CameraController * cameraController_;
};
