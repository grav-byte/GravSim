#pragma once
#include <string>

#include "imgui.h"
#include "IUserInterface.h"
#include "App/Layers/EngineLayer.h"


class SimulationUI : public IUserInterface {
public:
    SimulationUI();
    void OnEvent(Core::Event &event) override;

    void Draw() override;

private:
    void RunTest(const char *name);

    bool ImageBtn(const std::string &texturePath, bool disabled, const char *tooltip) const;

    ImVec4 btnBgColor_;
    ImVec4 btnTintColor_;
    ImVec4 btnDisabledColor_;
    EngineLayer* engine_;
    int activePropagatorIdx_;
    int stepsPerSec_;
    bool showPlot_;
    float plotHideTime_;
};
