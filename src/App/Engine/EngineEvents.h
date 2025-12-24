//
// Created by Lorenz Saalmann on 05.12.25.
//

#pragma once
#include "Scene.h"
#include "Core/Event.h"

class SceneLoadedEvent: public Core::Event {
public:
    Scene* GetScene() { return scene_; }
    Core::EventType GetEventType() const override { return Core::SceneLoaded; }

    explicit SceneLoadedEvent(Scene* scene): scene_(scene) {}
private:
    Scene* scene_;
};

class ObjectDestroyedEvent: public Core::Event {
public:
    uint32_t GetObjId() const { return objectId_; }
    Core::EventType GetEventType() const override { return Core::ObjectDestroyed; }

    explicit ObjectDestroyedEvent(const uint32_t objectId): objectId_(objectId) {}
private:
    uint32_t objectId_;
};
