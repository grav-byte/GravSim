#pragma once

#include "imgui.h"
#include "Core/AppLayer.h"

#include "App/UI/SceneUI.h"
#include "App/UI/SettingsUI.h"

class UILayer : public Core::AppLayer {
public:
    UILayer();
    ~UILayer() override;

    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnEvent(Core::Event &event) override;
    void OnRender() override;

    void AddUIElement(std::unique_ptr<IUserInterface> uiElement);

    static void DockWindowsFirstFrame(ImGuiID mainId);
    void DrawFPSCounter();

private:
    ImGuiIO* io_;

    std::vector<std::unique_ptr<IUserInterface>> uiElements_;
};
