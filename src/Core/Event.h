#pragma once

namespace Core {

    enum EventType {
        None = 0,
        WindowClosed, WindowResized,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
        SceneLoaded, ObjectDestroyed,
        SimulationStarted, SimulationPaused, SimulationStopped, SimulationResumed, SimulationStepped
    };

    class Event {
    public:
        virtual ~Event()=default;

        bool Handled = false;

        virtual EventType GetEventType() const = 0;
    };
}