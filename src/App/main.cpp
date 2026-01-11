#include "Layers/EngineLayer.h"
#include "Core/Application.h"
#include "Layers/AudioLayer.h"
#include "Layers/UILayer.h"
#include "RocketControl/ControlLayer.h"
#include "UI/FollowingUI.h"
#include "UI/SimulationUI.h"


int main() {
    Core::AppConfig config;
    config.Title = "Grav Sim";
    config.WindowConfig.Title = "Grav Sim";
    config.WindowConfig.Width = 1920;
    config.WindowConfig.Height = 1080;

    Core::Application app(config);

    app.PushLayer<EngineLayer>();
    app.PushLayer<AudioLayer>();
    app.PushLayer<ControlLayer>();
    app.PushLayer<UILayer>();

    if (UILayer* uiLayer = Core::Application::Get().GetLayer<UILayer>()) {
        // add uis
        uiLayer->AddUIElement(std::make_unique<SceneUI>());
        uiLayer->AddUIElement(std::make_unique<SimulationUI>());
        uiLayer->AddUIElement(std::make_unique<SettingsUI>());
        uiLayer->AddUIElement(std::make_unique<FollowingUI>());
    }

    app.Run();
}