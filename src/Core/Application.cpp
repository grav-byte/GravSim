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

    Application::Application(const AppConfig &app_config): m_Config(app_config) {
        if (s_Application) {
            throw std::runtime_error("Application already exists!");
        }
        s_Application = this;

        glfwSetErrorCallback(GLFWErrorCallback);
        glfwInit();

        m_Config.WindowConfig.EventCallback = [this](Event& event) { RaiseEvent(event); };

        m_Window = std::make_shared<Window>(m_Config.WindowConfig);
        m_Window->Create();
    }

    Application::~Application() {
        // destroy all layers first
        m_LayerStack.clear();

        // destroy the window
        if (m_Window)
            m_Window->Destroy();

        // terminate
        glfwTerminate();
        s_Application = nullptr;
    }

    Application & Application::Get() {
        return *s_Application;
    }

    void Application::Run() {
        m_Running = true;

        for (const std::unique_ptr<AppLayer>& layer : m_LayerStack)
            layer->OnInit();

        float lastTime = GetTime();

        // Main Application loop
        while (m_Running)
        {
            glfwPollEvents();

            if (m_Window->ShouldClose())
            {
                Stop();
                break;
            }

            float currentTime = GetTime();
            float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
            lastTime = currentTime;

            // Main layer update here
            for (const std::unique_ptr<AppLayer>& layer : m_LayerStack)
                layer->OnUpdate(timestep);

            // NOTE: rendering can be done elsewhere (eg. render thread)
            for (const std::unique_ptr<AppLayer>& layer : m_LayerStack)
                layer->OnRender();

            m_Window->SwapBuffers();
        }
    }

    void Application::Stop() {
        m_Running = false;
    }

    void Application::RaiseEvent(Event &event) const {
        // go through in reverse
        for (int i = static_cast<int>(m_LayerStack.size()) - 1; i >= 0; --i)
        {
            m_LayerStack[i]->OnEvent(event);
            if (event.Handled)
                break;
        }
    }

    float Application::GetTime()
    {
        return static_cast<float>(glfwGetTime());
    }
}
