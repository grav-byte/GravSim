#pragma once
#include "imgui.h"
#include "IUserInterface.h"
#include "App/Layers/EngineLayer.h"


class TimeControlUI : public IUserInterface {
public:
    TimeControlUI();
    void OnEvent(Core::Event &event) override;

    void Draw() override;

private:
    ImVec4 btnActiveColor_;
    EngineLayer* engine_;
};
