//
// Created by Lorenz Saalmann on 05.12.25.
//

#include "CameraController.h"

#include <iostream>

#include "EngineEvents.h"
#include "Core/InputEvents.h"

CameraController::CameraController() {
    camera_ = nullptr;
    shouldMove_ = false;
    currentMouseScreenPos_ = glm::vec2(0.0f, 0.0f);
    lastMouseWorldPos_ = glm::vec2(0.0f, 0.0f);

}

void CameraController::SetZoomToMouse(const bool enabled) {
    zoomToMouse_ = enabled;
}

void CameraController::OnEvent(Core::Event &event) {
    Core::EventType type = event.GetEventType();
    if (type == Core::SceneLoaded) {
        camera_ = dynamic_cast<SceneLoadedEvent&>(event).GetScene()->GetCamera();
    }

    if (type == Core::MouseButtonPressed) {
        shouldMove_ = true;
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
