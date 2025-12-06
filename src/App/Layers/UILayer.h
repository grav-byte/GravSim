//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once
#include "imgui.h"
#include "Core/AppLayer.h"
#include "../UI/SceneUI.h"
#include "../UI/SettingsUI.h"


class UILayer: public Core::AppLayer {
public:
    UILayer();
    ~UILayer();

    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnEvent(Core::Event &event) override;
    void OnRender() override;
private:
    bool showDemo_;
    ImGuiIO* io_;

    std::unique_ptr<SettingsUI> settingsUI_;
    std::unique_ptr<SceneUI> sceneUI_;
};
