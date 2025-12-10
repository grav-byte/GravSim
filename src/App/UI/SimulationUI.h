//
// Created by Lorenz Saalmann on 08.12.25.
//

#pragma once
#include <string>

#include "imgui.h"
#include "IUserInterface.h"
#include "App/Layers/EngineLayer.h"


class SimulationUI : public IUserInterface {
public:
    SimulationUI();
    void OnEvent(Core::Event &event) override;

    bool ImageBtn(const std::string &texturePath, bool disabled, const char *tooltip) const;

    void Draw() override;

private:
    ImVec4 btnBgColor_;
    ImVec4 btnTintColor_;
    ImVec4 btnDisabledColor_;
    EngineLayer* engine_;
    int activePropagatorIdx_;
};
