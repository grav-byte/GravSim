//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once
#include "Event.h"


namespace Core {
    class KeyEvent : public Event {
    public:
        int GetKeyCode() const { return m_KeyCode; }
        EventType GetEventType() const override { return KeyPressed; }
    protected:
        explicit KeyEvent(const int keycode): m_KeyCode(keycode) {}

        int m_KeyCode;
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

        MouseMovedEvent(const double x, const double y): m_MouseX(x), m_MouseY(y) {}

        double GetX() const { return m_MouseX; }
        double GetY() const { return m_MouseY; }

    private:
        double m_MouseX, m_MouseY;
    };

    class MouseButtonEvent : public Event
    {
    public:
        explicit MouseButtonEvent(const int button): m_Button(button) {}

        EventType GetEventType() const override { return MouseButtonPressed; }

        int GetMouseButton() const { return m_Button; }
    protected:
        int m_Button;
    };
}
