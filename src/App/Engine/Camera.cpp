//
// Created by Lorenz Saalmann on 02.12.25.
//

#include "Camera.h"

#include <iostream>

#include "Core/Application.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera() {
    transform = Transform();
    zoom = .1f;
    backgroundColor = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    // get aspect
    const auto bufferSize = Core::Application::Get().GetWindow()->GetFramebufferSize();
    const float aspect = static_cast<float>(bufferSize.x) / static_cast<float>(bufferSize.y);

    // use a projection to maintain aspect ratio
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
    // apply zoom
    projection = glm::scale(projection, glm::vec3(zoom, zoom, 1.0f));
    // apply position
    projection = glm::translate(projection, glm::vec3(-transform.position, 0.0f));
    return projection;
}

// convert a screen position (in pixels) to world position in meters
glm::vec2 Camera::ScreenToWorld(const glm::vec2& screenPos) const {
    const auto screenSize = Core::Application::Get().GetWindow()->GetFramebufferSize();
    const float aspect = static_cast<float>(screenSize.x) / static_cast<float>(screenSize.y);

    // how many meters fit in the screen
    float worldWidth = 2 * aspect / zoom;
    float worldHeight = 2.0f / zoom;

    // normalized coordinates [-1,1]
    float xNorm = 2 * screenPos.x / static_cast<float>(screenSize.x) - 1;
    float yNorm = -2 * screenPos.y / static_cast<float>(screenSize.y) + 1;

    float worldX = xNorm * worldWidth * .5f + transform.position.x;
    float worldY = yNorm * worldHeight * .5f + transform.position.y;
    return glm::vec2(worldX, worldY);
}

//
glm::vec2 Camera::WorldToScreen(const glm::vec2& worldPos) const {
    const auto screenSize = Core::Application::Get().GetWindow()->GetFramebufferSize();
    const float aspect = static_cast<float>(screenSize.x) / static_cast<float>(screenSize.y);

    // how many meters fit in the screen
    float worldWidth = 2 * aspect / zoom;
    float worldHeight = 2.0f / zoom;

    // world offset relative to camera center
    float xRel = worldPos.x - transform.position.x;
    float yRel = worldPos.y - transform.position.y;

    // normalized [-1, +1]
    float xNorm = (xRel / (worldWidth * 0.5f));
    float yNorm = (yRel / (worldHeight * 0.5f));

    // convert normalized to pixel coordinates
    float screenX = (xNorm + 1.0f) * 0.5f * screenSize.x;
    float screenY = (1.0f - yNorm) * 0.5f * screenSize.y;  // Y flipped

    return glm::vec2(screenX, screenY);
}
