//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once
#include <memory>
#include <vector>

#include "AppLayer.h"
#include "Window.h"

namespace Core {
    class Window;

    struct AppConfig {
        const char* Title = "ImGui Test";
        WindowConfig WindowConfig;
    };

    class Application {
    public:
        explicit Application(const AppConfig& app_config = AppConfig());
        ~Application();

        static Application& Get();

        void Run();
        void Stop();
        void RaiseEvent(Event& event) const;

        template<typename TLayer>
        void PushLayer()
        {
            static_assert(std::is_base_of_v<AppLayer, TLayer>, "TLayer must inherit from AppLayer");
            auto newLayer = std::make_unique<TLayer>();
            newLayer->SetWindow(m_Window.get());
            m_LayerStack.push_back(std::move(newLayer));
        }

        template<typename TLayer>
        TLayer* GetLayer()
        {
            static_assert(std::is_base_of_v<AppLayer, TLayer>, "TLayer must inherit from AppLayer");
            for (const auto& layer : m_LayerStack)
            {
                if (auto casted = dynamic_cast<TLayer*>(layer.get()))
                    return casted;
            }
            return nullptr;
        }

        static float GetTime();

    private:
        AppConfig m_Config;
        std::shared_ptr<Window> m_Window;
        bool m_Running = false;

        std::vector<std::unique_ptr<AppLayer>> m_LayerStack;
    };
}
