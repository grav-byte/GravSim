//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once
#include "imgui.h"
#include "Core/AppLayer.h"
#include "../UI/SceneUI.h"
#include "../UI/SettingsUI.h"
#include "App/UI/FollowingUI.h"
#include "App/UI/SimulationUI.h"


class UILayer: public Core::AppLayer {
public:
    UILayer();
    ~UILayer();

    void OnInit() override;


    void OnUpdate(float deltaTime) override;
    void OnEvent(Core::Event &event) override;
    void OnRender() override;

    static void DockWindowsFirstFrame(ImGuiID mainId);

    void DrawFPSCounter();

private:
    bool showDemo_;
    ImGuiIO* io_;

    std::unique_ptr<SettingsUI> settingsUI_;
    std::unique_ptr<SceneUI> sceneUI_;
    std::unique_ptr<SimulationUI> simulationUI_;
    std::unique_ptr<FollowingUI> followingUI_;
};
