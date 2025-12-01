#include "App/UILayer.h"
#include "Core/Application.h"

int main() {
    Core::AppConfig config;
    config.Title = "Window Demo";
    config.WindowConfig.Width = 1280;
    config.WindowConfig.Height = 720;
    Core::Application app(config);
    app.PushLayer<UILayer>();
    app.Run();

    return 0;
}
