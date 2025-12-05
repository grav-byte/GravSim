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
