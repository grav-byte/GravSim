#include "Window.h"
#ifdef _WIN32
#include <windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <stdexcept>
#include <iostream>

#include "InputEvents.h"
#include "WindowEvents.h"


struct ImGuiContext;

namespace Core {
    Window::Window(const WindowConfig& config): config_(config) {}

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




        handle_ = glfwCreateWindow(config_.Width, config_.Height, config_.Title, nullptr, nullptr);
        if (!handle_) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

#ifdef _WIN32
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101));
        if (hIcon)
        {
            HWND hwnd = glfwGetWin32Window(handle_); // requires GLFW_EXPOSE_NATIVE_WIN32
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
#endif

        glfwMakeContextCurrent(handle_);
        glfwSwapInterval(config_.VSync ? 1 : 0);

        glfwSetWindowUserPointer(handle_, this);

        glfwSetWindowCloseCallback(handle_, [](GLFWwindow* handle)
        {
            const Window& window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

            WindowClosedEvent event;
            window.RaiseEvent(event);
        });

        glfwSetWindowSizeCallback(handle_, [](GLFWwindow* handle, int width, int height)
        {
            const Window& window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));
            window.OnResize();

            WindowResizedEvent event(width, height);
            window.RaiseEvent(event);
        });

        glfwSetKeyCallback(handle_, [](GLFWwindow* handle, int key, int scancode, int action, int mods)
        {
            const Window& window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, false);
                    window.RaiseEvent(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, true);
                    window.RaiseEvent(event);break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    window.RaiseEvent(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(handle_, [](GLFWwindow* handle, int button, int action, int mods)
         {
             Window& window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

             switch (action)
             {
                 case GLFW_PRESS:
                 {
                     MouseButtonPressedEvent event(button);
                     window.RaiseEvent(event);
                     break;
                 }
                 case GLFW_RELEASE:
                 {
                     MouseButtonReleasedEvent event(button);
                     window.RaiseEvent(event);
                     break;
                 }

             }
         });

        glfwSetScrollCallback(handle_, [](GLFWwindow* handle, double, double yOffset) {
            const Window& window = *static_cast<Window*>(glfwGetWindowUserPointer(handle));

            MouseScrolledEvent event(static_cast<float>(yOffset));
            window.RaiseEvent(event);
        });

        glfwSetCursorPosCallback(handle_, [](GLFWwindow* handle, double x, double y)
        {
            const Window& window = *static_cast<Window *>(glfwGetWindowUserPointer(handle));

            float xscale = 1.0f;
            float yscale = 1.0f;
#ifdef __APPLE__
            // correct for display scaling - only needed on macOS
            glfwGetWindowContentScale(handle, &xscale, &yscale);
#endif

            MouseMovedEvent event(static_cast<float>(x * xscale), static_cast<float>(y * yscale));
            window.RaiseEvent(event);
        });
    }

    void Window::Destroy() {
        if (handle_) {
            glfwDestroyWindow(handle_);
            handle_ = nullptr;
        }
    }

    void Window::OnResize() const {
        const auto size = GetFramebufferSize();
        glViewport(0, 0, size.x, size.y);
    }

    void Window::RaiseEvent(Event& event) const {
        if (config_.EventCallback)
            config_.EventCallback(event);
    }

    void Window::SwapBuffers() const {
        if (!handle_) return;
        glfwSwapBuffers(handle_);
    }

    bool Window::ShouldClose() const {
        return handle_ ? glfwWindowShouldClose(handle_) : true;
    }

    glm::ivec2 Window::GetFramebufferSize() const {
        if (!handle_)
            return {0, 0};

        // Fallback: raw GLFW framebuffer size
        int width = 0, height = 0;
        glfwGetFramebufferSize(handle_, &width, &height);
        return { width, height };
    }
}
