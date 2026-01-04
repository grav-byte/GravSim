#pragma once
#include "Scene.h"
#include "Core/Event.h"

class SceneLoadedEvent: public Core::Event {
public:
    Scene* GetScene() const { return scene_; }
    Core::EventType GetEventType() const override { return Core::SceneLoaded; }

    explicit SceneLoadedEvent(Scene* scene): scene_(scene) {}
private:
    Scene* scene_;
};

class SimulationStartedEvent: public Core::Event {
public:
    Core::EventType GetEventType() const override { return Core::SimulationStarted; }

    explicit SimulationStartedEvent()= default;
};

class SimulationStoppedEvent: public Core::Event {
public:
    Core::EventType GetEventType() const override { return Core::SimulationStopped; }

    explicit SimulationStoppedEvent() = default;
};

class SimulationPausedEvent: public Core::Event {
public:
    Core::EventType GetEventType() const override { return Core::SimulationPaused; }

    explicit SimulationPausedEvent() = default;
};

class SimulationResumedEvent: public Core::Event {
public:
    Core::EventType GetEventType() const override { return Core::SimulationResumed; }

    explicit SimulationResumedEvent() = default;
};

class SimulationSteppedEvent: public Core::Event {
public:
    Core::EventType GetEventType() const override { return Core::SimulationStepped; }

    explicit SimulationSteppedEvent() = default;
};

class ObjectDestroyedEvent: public Core::Event {
public:
    uint32_t GetObjId() const { return objectId_; }
    Core::EventType GetEventType() const override { return Core::ObjectDestroyed; }

    explicit ObjectDestroyedEvent(const uint32_t objectId): objectId_(objectId) {}
private:
    uint32_t objectId_;
};
