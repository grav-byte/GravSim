//
// Created by Lorenz Saalmann on 27.11.25.
//

#include <iostream>

#include "Core/Application.h"

namespace Core {
    static Application* s_Application = nullptr;

    static void GLFWErrorCallback(int error, const char* description)
    {
        std::cerr << "[GLFW Error]: " << description << std::endl;
    }

    Application::Application(const AppConfig &app_config): config_(app_config) {
        if (s_Application) {
            throw std::runtime_error("Application already exists!");
        }
        s_Application = this;

        fs::create_directories(GetAppDataFolder());

        glfwSetErrorCallback(GLFWErrorCallback);
        glfwInit();

        config_.WindowConfig.EventCallback = [this](Event& event) { RaiseEvent(event); };

        window_ = std::make_shared<Window>(config_.WindowConfig);
        window_->Create();
    }

    Application::~Application() {
        // destroy all layers first
        layerStack_.clear();

        // destroy the window
        if (window_)
            window_->Destroy();

        // terminate
        glfwTerminate();
        s_Application = nullptr;
    }

    Application & Application::Get() {
        return *s_Application;
    }

    void Application::Run() {
        running_ = true;

        std::cout << "On Init" << std::endl;
        for (const std::unique_ptr<AppLayer>& layer : layerStack_)
            layer->OnInit();

        float lastTime = GetTime();

        // Main Application loop
        while (running_)
        {
            glfwPollEvents();

            if (window_->ShouldClose())
            {
                Stop();
                break;
            }

            float currentTime = GetTime();
            float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
            lastTime = currentTime;

            // Main layer update
            for (const std::unique_ptr<AppLayer>& layer : layerStack_)
                layer->OnUpdate(timestep);

            // NOTE: rendering can be done elsewhere (eg. render thread)
            for (const std::unique_ptr<AppLayer>& layer : layerStack_)
                layer->OnRender();

            window_->SwapBuffers();
        }
    }

    void Application::Stop() {
        running_ = false;
    }

    void Application::RaiseEvent(Event &event) const {
        // go through in reverse
        for (int i = static_cast<int>(layerStack_.size()) - 1; i >= 0; --i)
        {
            layerStack_[i]->OnEvent(event);
            if (event.Handled)
                break;
        }
    }

    float Application::GetTime()
    {
        return static_cast<float>(glfwGetTime());
    }

    fs::path Application::GetAppDataFolder() {
#ifdef _WIN32
        char* appdata = std::getenv("APPDATA");
        if (appdata) return fs::path(appdata) / "GravSim";
        return fs::path(".") / "GravSim"; // fallback
#elif __APPLE__
        const char* home = std::getenv("HOME");
        if (home) return fs::path(home) / "Library" / "Application Support" / "GravSim";
        return fs::path(".") / "GravSim";
#else // Linux/Unix
        const char* xdg = std::getenv("XDG_CONFIG_HOME");
        if (xdg) return fs::path(xdg) / "GravSim";
        const char* home = std::getenv("HOME");
        if (home) return fs::path(home) / ".config" / "GravSim";
        return fs::path(".") / "GravSim";
#endif
    }
}
