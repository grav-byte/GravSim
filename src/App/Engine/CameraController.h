//
// Created by Lorenz Saalmann on 05.12.25.
//

#pragma once
#include "Camera.h"
#include "EngineEvents.h"
#include "SceneObject.h"
#include "Core/Event.h"


class CameraController {
public:
    CameraController();
    void SetZoomToMouse(bool enabled);

    void Update(float deltaTime) const;

    void HandleObjectFollowSelection();

    void OnEvent(Core::Event &event);

    SceneObject* GetFollowingObject() const;

    void StopFollowing();

private:
    Camera* camera_;
    bool zoomToMouse_;
    glm::vec2 lastMouseWorldPos_;
    bool shouldMove_;
    glm::vec2 currentMouseScreenPos_;
    SceneObject* followObj_;
    Scene * scene_;
    float lastClickTime_;
    float doubleClickThreshold_;
};
