//
// Created by Lorenz Saalmann on 02.12.25.
//

#pragma once
#include "Transform.h"
#include "glm/fwd.hpp"


class Camera {
public:
    Camera();

    Transform transform;
    float zoom;
    glm::vec4 backgroundColor;

    glm::mat4 GetProjectionMatrix() const;
    glm::vec2 ScreenToWorld(const glm::vec2& screenPos) const;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(transform, zoom, backgroundColor);
    }
};
