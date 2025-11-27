//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once

#include "Event.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>

namespace Core {

    struct WindowConfig
    {
        const char* Title = "Window";
        int Width = 1280;
        int Height = 720;
        bool IsResizeable = true;
        bool VSync = true;

        std::function<void(Event&)> EventCallback;
    };

    class Window {
    public:
        explicit Window(const WindowConfig& config);
        ~Window();

        void Create();
        void Destroy();

        void RaiseEvent(Event &event) const;

        void SwapBuffers() const;
        bool ShouldClose() const;

        glm::ivec2 GetFramebufferSize() const;

        GLFWwindow* GetHandle() const { return m_Handle; }

    private:
        WindowConfig m_Config;
        GLFWwindow* m_Handle = nullptr;
    };

}