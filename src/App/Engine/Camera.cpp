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
    backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
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
