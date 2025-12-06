//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once
#include "Event.h"


namespace Core {
    class KeyEvent : public Event {
    public:
        int GetKeyCode() const { return keyCode_; }
        EventType GetEventType() const override { return KeyPressed; }
    protected:
        explicit KeyEvent(const int keycode): keyCode_(keycode) {}

        int keyCode_;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(const int keycode): KeyEvent(keycode) {}
        EventType GetEventType() const override { return KeyReleased; }
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(const int keycode, const bool isRepeat): KeyEvent(keycode), m_IsRepeat(isRepeat) {}
        EventType GetEventType() const override { return KeyPressed; }

        bool IsRepeat() const { return m_IsRepeat; }
    private:
        bool m_IsRepeat;
    };

    class MouseMovedEvent : public Event
    {
    public:
        EventType GetEventType() const override { return MouseMoved; }

        MouseMovedEvent(const float x, const float y): mouseX_(x), mouseY_(y) {}

        float GetX() const { return mouseX_; }
        float GetY() const { return mouseY_; }

    private:
        float mouseX_, mouseY_;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        EventType GetEventType() const override { return MouseScrolled; }

        MouseScrolledEvent(const float amount): scrollAmount_(amount) {}

        float GetAmount() const { return scrollAmount_; }

    private:
        float scrollAmount_;
    };

    class MouseButtonEvent : public Event
    {
    public:
        int GetMouseButton() const { return button_; }
    protected:
        explicit MouseButtonEvent(const int button) : button_(button) {}
        int button_;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressedEvent(const int button): MouseButtonEvent(button) {}
        EventType GetEventType() const override { return MouseButtonPressed; }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button): MouseButtonEvent(button) {}
        EventType GetEventType() const override { return MouseButtonReleased; }
    };
}
