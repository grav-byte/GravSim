//
// Created by Lorenz Saalmann on 05.12.25.
//

#include "CameraController.h"

#include <iostream>

#include "EngineEvents.h"
#include "Core/Application.h"
#include "Core/InputEvents.h"

CameraController::CameraController() {
    camera_ = nullptr;
    shouldMove_ = false;
    zoomToMouse_ = true;
    currentMouseScreenPos_ = glm::vec2(0.0f, 0.0f);
    lastMouseWorldPos_ = glm::vec2(0.0f, 0.0f);

    doubleClickThreshold_ = 0.3f; // 300 ms for double click
    lastClickTime_ = -doubleClickThreshold_;
    followObj_ = nullptr;
}

void CameraController::SetZoomToMouse(const bool enabled) {
    zoomToMouse_ = enabled;
}

void CameraController::Update(float deltaTime) const {
    if (followObj_ && camera_) {
        camera_->transform.position = followObj_->transform.position;
    }
}

void CameraController::HandleObjectFollowSelection() {
    const float now = Core::Application::GetTime();

    if (now - lastClickTime_ < doubleClickThreshold_) {
        shouldMove_ = false;

        for (const auto obj : scene_->GetAllObjects()) {
            glm::vec2 objScreenPos = camera_->WorldToScreen(obj->transform.position);
            float distance = glm::length(currentMouseScreenPos_ - objScreenPos);
            // threshold in pixels
            if (distance < 100.0f) {
                followObj_ = obj;
                break;
            }
        }
    }

    // Update timestamp
    lastClickTime_ = now;
}

void CameraController::OnEvent(Core::Event &event) {
    Core::EventType type = event.GetEventType();
    if (type == Core::SceneLoaded) {
        scene_ = dynamic_cast<SceneLoadedEvent&>(event).GetScene();
        followObj_ = nullptr;
        camera_ = scene_->GetCamera();
    }

    if (type == Core::MouseButtonPressed) {
        shouldMove_ = true;

        HandleObjectFollowSelection();
    }

    if (type == Core::MouseButtonReleased) {
        shouldMove_ = false;
        lastMouseWorldPos_ = glm::vec2(0.0f, 0.0f);
    }

    if (!camera_)
        return;

    if (type == Core::MouseMoved) {
        const auto& mouseEvent = dynamic_cast<Core::MouseMovedEvent&>(event);
        currentMouseScreenPos_ = glm::vec2(mouseEvent.GetX(), mouseEvent.GetY());
        // convert to world space
        glm::vec2 currentMousePos = camera_->ScreenToWorld(currentMouseScreenPos_);

        if (shouldMove_ && lastMouseWorldPos_ != glm::vec2(0.0f, 0.0f)) {
            camera_->transform.position += lastMouseWorldPos_ - currentMousePos;
        }

        lastMouseWorldPos_ = camera_->ScreenToWorld(currentMouseScreenPos_);
    }

    if (type == Core::MouseScrolled) {
        const auto& scrollEvent = dynamic_cast<Core::MouseScrolledEvent&>(event);

        // zoom in towards cursor
        glm::vec2 cursorWorldPosBefore = camera_->ScreenToWorld(currentMouseScreenPos_);

        float factor = pow(1.1f, scrollEvent.GetAmount());
        // clamp zoom
        camera_->zoom = std::max(camera_->zoom * factor, 0.01f);

        // adjust position to keep cursor in place
        if (zoomToMouse_) {
            glm::vec2 cursorWorldPosAfter = camera_->ScreenToWorld(currentMouseScreenPos_);
            camera_->transform.position += cursorWorldPosBefore - cursorWorldPosAfter;
        } else {
            lastMouseWorldPos_ = camera_->ScreenToWorld(currentMouseScreenPos_);
        }
    }
}

SceneObject * CameraController::GetFollowingObject() const {
    return followObj_;
}

void CameraController::StopFollowing() {
    followObj_ = nullptr;
}
