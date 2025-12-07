//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once
#include <memory>
#include <vector>

#include <filesystem>
namespace fs = std::filesystem;

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
            newLayer->SetWindow(window_);
            layerStack_.push_back(std::move(newLayer));
        }

        template<typename TLayer>
        TLayer* GetLayer()
        {
            static_assert(std::is_base_of_v<AppLayer, TLayer>, "TLayer must inherit from AppLayer");

            for (const auto& layer : layerStack_)
            {
                // dynamic_cast checks runtime type; returns nullptr if not TLayer
                if (auto casted = dynamic_cast<TLayer*>(layer.get()))
                    return casted;
            }

            return nullptr;
        }

        std::shared_ptr<Window> GetWindow() const { return window_; }

        static float GetTime();

        static fs::path GetAppDataFolder();

    private:
        AppConfig config_;
        std::shared_ptr<Window> window_;
        bool running_ = false;

        std::vector<std::unique_ptr<AppLayer>> layerStack_;
    };
}
