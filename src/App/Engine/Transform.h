//
// Created by Lorenz Saalmann on 05.12.25.
//

#pragma once
#include <glm/vec2.hpp>

#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"

struct Transform {
    glm::vec2 position = glm::vec2(0.0f, 0.0f);
    float rotation = 0.0f; // in degrees
    glm::vec2 scale = glm::vec2(1.0f, 1.0f);

    glm::mat4 GetMatrix() const {
        auto transform = glm::mat4(1.0f);
        // position
        transform = glm::translate(transform, glm::vec3(position, 0.0f));

        // rotation
        transform = glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

        // scale: scale by radius and scale vector
        transform = glm::scale(transform, glm::vec3(scale, 1.0f));
        return transform;
    }

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(position, rotation, scale);
    }
};
