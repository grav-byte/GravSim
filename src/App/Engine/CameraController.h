//
// Created by Lorenz Saalmann on 05.12.25.
//

#pragma once
#include "Camera.h"
#include "Core/Event.h"


class CameraController {
public:
    CameraController();
    void OnEvent(Core::Event &event);
private:
    Camera* camera_;
    glm::vec2 lastMouseWorldPos_;
    bool shouldMove_;
    glm::vec2 currentMouseScreenPos_;
};
