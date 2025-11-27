//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once
#include "Event.h"


namespace Core {
    class WindowClosedEvent : public Event {
    public:
        EventType GetEventType() const override { return WindowClosed; }
    };

    class WindowResizedEvent : public Event {
    public:
        WindowResizedEvent(int width, int height) : m_Width(width), m_Height(height) {}
        EventType GetEventType() const override { return WindowResized; }
    private:
        int m_Width;
        int m_Height;
    };
}
