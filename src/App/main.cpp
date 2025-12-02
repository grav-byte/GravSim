#include "EngineLayer.h"
#include "UILayer.h"
#include "../Core/Application.h"

int main() {
    Core::AppConfig config;
    config.Title = "Window Demo";
    config.WindowConfig.Width = 1280;
    config.WindowConfig.Height = 720;
    Core::Application app(config);
    app.PushLayer<EngineLayer>();
    app.PushLayer<AudioLayer>();
    app.PushLayer<UILayer>();
    app.Run();
}
