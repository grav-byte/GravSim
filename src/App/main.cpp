#include "Layers/EngineLayer.h"
#include "../Core/Application.h"
#include "Layers/AudioLayer.h"
#include "Layers/UILayer.h"

int main() {
    Core::AppConfig config;
    config.Title = "Grav Sim";
    config.WindowConfig.Title = "Grav Sim";
    config.WindowConfig.Width = 1920;
    config.WindowConfig.Height = 1080;

    Core::Application app(config);

    app.PushLayer<EngineLayer>();
    app.PushLayer<AudioLayer>();
    app.PushLayer<UILayer>();

    app.Run();
}
