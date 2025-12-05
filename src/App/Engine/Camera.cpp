//
// Created by Lorenz Saalmann on 02.12.25.
//

#include "Camera.h"

#include "Core/Application.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera() {
    transform = Transform();
    zoom = 1;
}

glm::mat4 Camera::GetProjectionMatrix() const {
    // get aspect
    const auto bufferSize = Core::Application::Get().GetWindow()->GetFramebufferSize();
    const float aspect = static_cast<float>(bufferSize.x) / static_cast<float>(bufferSize.y);

    // use a projection to maintain aspect ratio
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
    // apply zoom
    projection = glm::scale(projection, glm::vec3(zoom * .1f, zoom * .1f, 1.0f));
    // apply position
    projection = glm::translate(projection, glm::vec3(-transform.position, 0.0f));
    return projection;
}
