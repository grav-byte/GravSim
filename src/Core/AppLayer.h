//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once

#include "Window.h"
#include "WindowEvents.h"


namespace Core {
    class Window;

    class AppLayer {
    public:
        AppLayer() = default;

        void SetWindow(Window* window) {
            m_Window = std::shared_ptr<Window>(window);
        }

        virtual void OnInit() = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnEvent(Event& event) = 0;
        virtual void OnRender() = 0;

    protected:
        std::shared_ptr<Window> m_Window;
    };
}
