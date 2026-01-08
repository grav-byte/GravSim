#pragma once

#include "imgui.h"
#include "Core/AppLayer.h"

#include "App/UI/SceneUI.h"
#include "App/UI/SettingsUI.h"
#include "App/UI/FollowingUI.h"
#include "App/UI/SimulationUI.h"
#include "App/UI/InteractionState.h"
#include "App/UI/InteractionUI.h"
#include "App/UI/RocketControllerUI.h"
#include "App/UI/RocketStateUI.h"

class UILayer : public Core::AppLayer {
public:
    UILayer();
    ~UILayer() override;

    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnEvent(Core::Event &event) override;
    void OnRender() override;

    static void DockWindowsFirstFrame(ImGuiID mainId);
    void DrawFPSCounter();

private:
    ImGuiIO* io_;

    std::unique_ptr<SettingsUI> settingsUI_;
    std::unique_ptr<SceneUI> sceneUI_;
    std::unique_ptr<SimulationUI> simulationUI_;
    std::unique_ptr<FollowingUI> followingUI_;

    InteractionState interactionState_;
    std::unique_ptr<InteractionUI> interactionUI_;
    std::unique_ptr<RocketStateUI>  stateUI_;
    std::unique_ptr<RocketControllerUI>  controlUI_;
};
