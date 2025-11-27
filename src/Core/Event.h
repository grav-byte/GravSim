//
// Created by Lorenz Saalmann on 27.11.25.
//

#pragma once

namespace Core {

    enum EventType {
        None = 0,
        WindowClosed, WindowResized,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseMoved
    };

    class Event {
    public:
        virtual ~Event()=default;

        bool Handled = false;

        virtual EventType GetEventType() const = 0;
    };
}