//
// Created by Lorenz Saalmann on 27.11.25.
//

#include "Window.h"
#include <stdexcept>
#include <iostream>

#include "InputEvents.h"
#include "WindowEvents.h"

namespace Core {
    Window::Window(const WindowConfig& config): m_Config(config) {}

    Window::~Window() {
        Destroy();
    }

    void Window::Create() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

#ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS compatibility
#endif

        m_Handle = glfwCreateWindow(m_Config.Width, m_Config.Height, m_Config.Title, nullptr, nullptr);
        if (!m_Handle) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_Handle);
        glfwSwapInterval(m_Config.VSync ? 1 : 0);

        glfwSetWindowUserPointer(m_Handle, this);

        glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* handle)
        {
            Window& window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

            WindowClosedEvent event;
            window.RaiseEvent(event);
        });

        glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* handle, int width, int height)
        {
            Window& window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

            WindowResizedEvent event(width, height);
            window.RaiseEvent(event);
        });

        glfwSetKeyCallback(m_Handle, [](GLFWwindow* handle, int key, int scancode, int action, int mods)
        {
            Window& window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

            switch (action)
            {
                case GLFW_PRESS:
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, action == GLFW_REPEAT);
                    window.RaiseEvent(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    window.RaiseEvent(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* handle, int button, int action, int mods)
        {
            Window& window = *((Window*)glfwGetWindowUserPointer(handle));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonEvent event(button);
                    window.RaiseEvent(event);
                    break;
                }
            }
        });

        glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* handle, double x, double y)
        {
            Window& window = *((Window*)glfwGetWindowUserPointer(handle));

            MouseMovedEvent event(x, y);
            window.RaiseEvent(event);
        });
    }

    void Window::Destroy() {
        if (m_Handle) {
            glfwDestroyWindow(m_Handle);
            m_Handle = nullptr;
        }
    }

    void Window::RaiseEvent(Event& event) const {
        if (m_Config.EventCallback)
            m_Config.EventCallback(event);
    }

    void Window::SwapBuffers() const {
        if (!m_Handle) return;
        glfwSwapBuffers(m_Handle);
    }

    bool Window::ShouldClose() const {
        return m_Handle ? glfwWindowShouldClose(m_Handle) : true;
    }

    glm::ivec2 Window::GetFramebufferSize() const {
        if (!m_Handle) return {0, 0};
        int width, height;
        glfwGetFramebufferSize(m_Handle, &width, &height);
        return {width, height};
    }
}
