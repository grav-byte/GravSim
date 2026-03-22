#pragma once
#include "glm/vec4.hpp"
class RenderingSystem;

enum class VisualType {
    Circle,
    Sprite,
    Shader
};

class IVisual {
public:
    virtual ~IVisual() = default;
    glm::vec4 color = glm::vec4(1.0f);
    int drawOrder = 0; // lower drawn first

    virtual VisualType GetType() = 0;

    template<class Archive>
    void serialize(Archive& ar) {
        ar(drawOrder);
    }
};

